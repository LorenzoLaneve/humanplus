// => src/frontend/invocation.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/frontend/invocation.h>
#include <hpc/frontend/instance.h>
#include <hpc/utils/opts.h>
#include <hpc/utils/files.h>

#include <llvm/ADT/Triple.h>
#include <llvm/MC/SubtargetFeature.h>
#include <llvm/Option/ArgList.h>

using namespace hpc;

static bool parseFrontendArgs(CompilerInvocation &invoke, diag::DiagEngine &diags, llvm::opt::InputArgList &args) {
    
    opts::FrontendOptions &frontendOpts = invoke.getFrontendOptions();
    
    frontendOpts.showHelp = args.hasArg(opts::__help);
    frontendOpts.showVersion = args.hasArg(opts::__version);
    
    frontendOpts.outputFile = args.getLastArgValue(opts::o);
    
    for (std::string input : args.getAllArgValues(opts::InputFiles)) {
        fsys::InputFile *ifile = fsys::InputFile::fromFile(input);
        
        if (!ifile->exists()) {
            diags.reportDiag(diag::Error, diag::NoSuchFileOrDirectory) << input;
            continue;
        }
        
        frontendOpts.inputFiles.push_back(ifile);
    }
    
    return true;
}

static bool parseTargetArgs(CompilerInvocation &invoke, diag::DiagEngine &diags, llvm::opt::InputArgList &args) {
    
    opts::TargetOptions &targetOpts = invoke.getTargetOptions();
    
    if (llvm::opt::Arg *A = args.getLastArg(opts::triple)) {
        targetOpts.targetTriple = llvm::Triple::normalize(A->getValue());
    }
    
    if (targetOpts.targetTriple.empty()) {
        targetOpts.targetTriple = llvm::sys::getDefaultTargetTriple();
    }
    
    llvm::Triple theTriple(targetOpts.targetTriple);
    
    targetOpts.CPU = args.getLastArgValue(opts::target_cpu);
    //targetOpts.features....
    llvm::SubtargetFeatures targetFeatures;
    
    
    return true;
}

bool CompilerInvocation::create(CompilerInstance &hpc, const char **argBegin, const char **argEnd, diag::DiagEngine &diags) {
    opts::OptionsTable *opttable = opts::getTable();
    
    unsigned missingArgIndex;
    unsigned missingArgCount;
    
    llvm::opt::InputArgList args = opttable->ParseArgs(llvm::makeArrayRef(argBegin, argEnd), missingArgIndex, missingArgCount);
    
    bool success = true;
    
    if (missingArgCount) {
        if (missingArgCount > 1) {
            diags.reportDiag(diag::Error, diag::MoreArgumentsToOptionAreMissing) << args.getArgString(missingArgIndex) << missingArgCount;
        } else {
            diags.reportDiag(diag::Error, diag::ArgumentToOptionIsMissing) << args.getArgString(missingArgIndex);
        }
        success = false;
    }
    
    for (const llvm::opt::Arg *arg : args.filtered(opts::Unknown)) {
        diags.reportDiag(diag::Error, diag::UnknownArgument) << arg->getAsString(args);
        success = false;
    }
    
    CompilerInvocation *invoke = new CompilerInvocation(hpc);

    parseFrontendArgs(*invoke, diags, args);
    parseTargetArgs(*invoke, diags, args);
    
    
    hpc.setInvocation(*invoke);
    return success;
}
