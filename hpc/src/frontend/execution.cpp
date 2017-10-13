// => src/frontend/execution.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/frontend/instance.h>
#include <hpc/frontend/invocation.h>
#include <hpc/utils/opts.h>
#include <hpc/utils/printers.h>
#include <hpc/utils/files.h>
#include <hpc/extras/dump/dump.h>
#include <hpc/ast/unit.h>
#include <hpc/ast/builder/builder.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/drivers/driver.h>
#include <hpc/drivers/system/system.h>
#include <hpc/analyzers/sources.h>
#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/analyzers/parser/parser.h>
#include <hpc/analyzers/validator/validator.h>
#include <hpc/ir/modules.h>
#include <hpc/ir/builders.h>
#include <hpc/target/target.h>
#include <hpc/backend/backend.h>
#include <hpc/linker/linker.h>

#include <string>
#include <vector>

using namespace hpc;

bool hpc::CompilerInstance::executeInvocation() {
    
    opts::FrontendOptions &frontendOpts = getFrontendOptions();
    
    // --help
    if (frontendOpts.showHelp) {
        opts::getTable()->PrintHelp(llvm::outs(), "hpc", "Human Plus LLVM Compiler");
        return true;
    }
    
    // --version
    if (frontendOpts.showVersion) {
        llvm::cl::PrintVersionMessage(); // FIXME
        return true;
    }
    
    
    if (frontendOpts.inputFiles.empty()) {
        diags->reportError(diag::NoInputFiles);
        return false;
    }


    ast::AbstractSyntaxTree *AST = new ast::AbstractSyntaxTree();
    
    target::TargetInfo *targetInfo = target::TargetInfo::fromOptions(getTargetOptions(), getDiagnostics());
    
    if(!targetInfo->createTargetABI()) {
        getDiagnostics().reportError(diag::ABICreationFailed);
        return false;
    }
    
    std::vector<source::SourceFile *> sourcefiles;
    
    parser::ParserInstance parser(getDiagnostics(), AST);
    for (fsys::File *ifile : frontendOpts.inputFiles) {
        if (ifile->getType() == fsys::SourceFile) {
            if (parser.bindSourceFile(static_cast<source::SourceFile *>(ifile))) {
                parser.parse();
            
                sourcefiles.push_back(parser.getSourceFile());
                parser.unbindSourceFile();
            }
        }
    }
    
    if (getDiagnostics().getErrorCount()) return false;
    
    validator::ValidatorInstance validator(getDiagnostics());
    
    validator.validate(AST);
    
#ifdef __hpc_fe_ast_debug
    extras::dumpAST(AST);
#endif
    
    if (getDiagnostics().getErrorCount()) return false;
    
    for (source::SourceFile *src : sourcefiles)
        if (ast::CompilationUnit *theUnit = AST->getUnitForFile(src)) {
            codegen::ModuleBuilder builder(*src->getModuleWrapper(), *targetInfo);
            builder.buildUnit(theUnit);
            
            src->getModuleWrapper()->finalize();
            src->getModuleWrapper()->getModule().setTargetTriple(targetInfo->getTriple());
        }
    
    fsys::FileType backendoutput = frontendOpts.outputType;
    
    if (backendoutput != fsys::Nothing) {
        if (backendoutput == fsys::Executable)
            backendoutput = fsys::ObjectCode;
        
        opts::LinkOptions linkopts;
        linkopts.outputFile = frontendOpts.outputFile;
        
        backend::init(getDiagnostics(), getBackendOptions(), getTargetOptions());
        
        backend::BackendHelper backend(getDiagnostics(), getBackendOptions(), targetInfo);
        backend.bindTarget(targetInfo);
        
        for (source::SourceFile *src : sourcefiles) {
            backend.bindModule(src->getModuleWrapper()->getModule());
            
            backend.runPasses();
            
            std::error_code errcode;
            fsys::OutputFile *outfile = fsys::OutputFile::create(src->getFileName(), backendoutput, errcode, frontendOpts.outputType == fsys::Executable);
            
            if (!backend.emit(outfile)) {
                llvm::errs() << "compilation failed.";
                return 1;
            }
            
            if (frontendOpts.outputType == fsys::Executable) {
                linkopts.inputFiles.push_back(outfile);
            }
        }
        
#ifdef __hpc_fe_ir_debug
        for (source::SourceFile *src : sourcefiles) src->getModuleWrapper()->dump();
#endif
        
        if (frontendOpts.outputType == fsys::Executable) {
            if (int linkstatus = drivers::getHostSystem()->invokeLinker(linkopts)) {
                getDiagnostics().reportError(diag::LinkerFailedWithExitCode) << linkstatus;
                return linkstatus;
            }
        }
    }
    
    return true;
}
