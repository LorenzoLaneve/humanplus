//
//  objsynth.cpp
//  HumanPlus
//
//  Created by Lore on 19/03/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/backend/backend.h>
#include <hpc/target/target.h>
#include <hpc/utils/opts.h>
#include <hpc/linker/input.h>

#include <llvm/ADT/StringExtras.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Analysis/TargetTransformInfo.h>
#include <llvm/Bitcode/BitcodeWriterPass.h>
#include <llvm/CodeGen/LinkAllAsmWriterComponents.h>
#include <llvm/CodeGen/LinkAllCodegenComponents.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Pass.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetSubtargetInfo.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <memory>

#define FileType llvm::TargetMachine::CGFT_ObjectFile   // FIXME: -object file type by flag
#define NoIntegratedAssembler false                     // FIXME: -no-integrated-as by flag
#define FShowMCEncoding false                           // FIXME: -show-mc-encoding by flag
#define EnableDwarfDirectory false                      // FIXME: -enable-dwarf-directory by flag
#define FAsmVerbose true                                // FIXME: -asm-verbose by flag
#define FRelocModel llvm::Reloc::Default                // FIXME: -relocation-model by flag
#define FCMModel llvm::CodeModel::Default               // FIXME: -code-model by flag
#define DisableSimplifyLibCalls false                   // FIXME: -disable-simplify-libcalls by flag
#define CompileTwice false                              // FIXME: -compile-twice by flag
#define NoVerify false                                  // FIXME: -no-verify by flag

using namespace hpc;


static llvm::TargetIRAnalysis getTargetIRAnalysis(llvm::TargetMachine *machine) {
    return machine->getTargetIRAnalysis();
}

static void addAddDiscriminatorsPass(const llvm::PassManagerBuilder &pmb, llvm::legacy::PassManagerBase &manager) {
    manager.add(llvm::createAddDiscriminatorsPass());
}

backend::BackendHelper::BackendHelper(diag::DiagEngine &diags, opts::BackendOptions &backendOptions, target::TargetInfo *targetInfo)
: diags(diags), backendOptions(backendOptions), targetOptions(targetInfo->getOptions()), targetInfo(targetInfo) {  }


void backend::BackendHelper::bindTarget(target::TargetInfo *target) {
    targetInfo = target;
    
    managerBuilder.LibraryInfo = target->createTLII();
}

void backend::BackendHelper::bindModule(llvm::Module &module) {
    boundmodule = &module;
    
    bool machineCreated = getTargetInfo().createTargetMachine(backendOptions, diags);
    
    llvm::TargetMachine *targetMachine = getTargetInfo().getTargetMachine();
    
    if (machineCreated) {
        module.setDataLayout(targetMachine->createDataLayout());
    }
    
    
    // FIXME add inliner passes flag
    managerBuilder.Inliner = llvm::createAlwaysInlinerPass();
    
    managerBuilder.OptLevel = backendOptions.optimizationLevel;
    managerBuilder.SizeLevel = backendOptions.sizeLevel;
//    managerBuilder.BBVectorize = ...;
//    managerBuilder.SLPVectorize = ...;
//    managerBuilder.LoopVectorize = ...;
//    
//    managerBuilder.DisableUnrollLoops = ...;
//    managerBuilder.MergeFunctions = ...;
//    managerBuilder.PrepareForThinLTO = ...;
//    managerBuilder.PrepareForLTO = ...;
//    managerBuilder.RerollLoops = ...;
    
   // If we are performing a ThinLTO importing compile, invoke the LTO
   // pipeline and pass down the in-memory module summary index.
//    if (ModuleSummary) {
//        managerBuilder.ModuleSummary = ModuleSummary;
//        managerBuilder.populateThinLTOPassManager(*mpm);
//        return;
//    }
    
    // Add target-specific passes that need to run as early as possible.
//    if (llvm::TargetMachine *tmachine = target::getMachine())
//        managerBuilder.addExtension(llvm::PassManagerBuilder::EP_EarlyAsPossible,
//                                    [&](const llvm::PassManagerBuilder &, llvm::legacy::PassManagerBase &PM) {
//                                   tmachine->addEarlyAsPossiblePasses(PM);
//                               });
    
    managerBuilder.addExtension(llvm::PassManagerBuilder::EP_EarlyAsPossible, addAddDiscriminatorsPass);
    
//    if (LangOpts.Sanitize.has(SanitizerKind::LocalBounds)) {
//        PMBuilder.addExtension(PassManagerBuilder::EP_ScalarOptimizerLate,
//                               addBoundsCheckingPass);
//        PMBuilder.addExtension(PassManagerBuilder::EP_EnabledOnOptLevel0,
//                               addBoundsCheckingPass);
//    }
//    
//    if (CodeGenOpts.SanitizeCoverageType ||
//        CodeGenOpts.SanitizeCoverageIndirectCalls ||
//        CodeGenOpts.SanitizeCoverageTraceCmp) {
//        PMBuilder.addExtension(PassManagerBuilder::EP_OptimizerLast,
//                               addSanitizerCoveragePass);
//        PMBuilder.addExtension(PassManagerBuilder::EP_EnabledOnOptLevel0,
//                               addSanitizerCoveragePass);
//    }
//    
//    if (LangOpts.Sanitize.has(SanitizerKind::Address)) {
//        PMBuilder.addExtension(PassManagerBuilder::EP_OptimizerLast,
//                               addAddressSanitizerPasses);
//        PMBuilder.addExtension(PassManagerBuilder::EP_EnabledOnOptLevel0,
//                               addAddressSanitizerPasses);
//    }
//    
//    if (LangOpts.Sanitize.has(SanitizerKind::KernelAddress)) {
//        PMBuilder.addExtension(PassManagerBuilder::EP_OptimizerLast,
//                               addKernelAddressSanitizerPasses);
//        PMBuilder.addExtension(PassManagerBuilder::EP_EnabledOnOptLevel0,
//                               addKernelAddressSanitizerPasses);
//    }
//    
//    if (LangOpts.Sanitize.has(SanitizerKind::Memory)) {
//        PMBuilder.addExtension(PassManagerBuilder::EP_OptimizerLast,
//                               addMemorySanitizerPass);
//        PMBuilder.addExtension(PassManagerBuilder::EP_EnabledOnOptLevel0,
//                               addMemorySanitizerPass);
//    }
//    
//    if (LangOpts.Sanitize.has(SanitizerKind::Thread)) {
//        PMBuilder.addExtension(PassManagerBuilder::EP_OptimizerLast,
//                               addThreadSanitizerPass);
//        PMBuilder.addExtension(PassManagerBuilder::EP_EnabledOnOptLevel0,
//                               addThreadSanitizerPass);
//    }
//    
//    if (LangOpts.Sanitize.has(SanitizerKind::DataFlow)) {
//        PMBuilder.addExtension(PassManagerBuilder::EP_OptimizerLast,
//                               addDataFlowSanitizerPass);
//        PMBuilder.addExtension(PassManagerBuilder::EP_EnabledOnOptLevel0,
//                               addDataFlowSanitizerPass);
//    }
//    
//    if (LangOpts.Sanitize.hasOneOf(SanitizerKind::Efficiency)) {
//        PMBuilder.addExtension(PassManagerBuilder::EP_OptimizerLast,
//                               addEfficiencySanitizerPass);
//        PMBuilder.addExtension(PassManagerBuilder::EP_EnabledOnOptLevel0,
//                               addEfficiencySanitizerPass);
//    }
    
    // Set up the per-function pass manager.
    fpm = new llvm::legacy::FunctionPassManager(&module);
    
//  if (CodeGenOpts.VerifyModule)
        fpm->add(llvm::createVerifierPass());
    
    // Set up the per-module pass manager.
    mpm = new llvm::legacy::PassManager();
    
    
    if (machineCreated) {
        fpm->add(llvm::createTargetTransformInfoWrapperPass(getTargetIRAnalysis(targetMachine)));
        mpm->add(llvm::createTargetTransformInfoWrapperPass(getTargetIRAnalysis(targetMachine)));
    }
    
// TODO Symbol rewriter pass
    
//    if (!CodeGenOpts.DisableGCov && (CodeGenOpts.EmitGcovArcs || CodeGenOpts.EmitGcovNotes)) {
//        // Not using 'GCOVOptions::getDefault' allows us to avoid exiting if
//        // LLVM's -default-gcov-version flag is set to something invalid.
//        GCOVOptions Options;
//        Options.EmitNotes = CodeGenOpts.EmitGcovNotes;
//        Options.EmitData = CodeGenOpts.EmitGcovArcs;
//        memcpy(Options.Version, CodeGenOpts.CoverageVersion, 4);
//        Options.UseCfgChecksum = CodeGenOpts.CoverageExtraChecksum;
//        Options.NoRedZone = CodeGenOpts.DisableRedZone;
//        Options.FunctionNamesInData =
//        !CodeGenOpts.CoverageNoFunctionNamesInData;
//        Options.ExitBlockBeforeBody = CodeGenOpts.CoverageExitBlockBeforeBody;
//        MPM->add(createGCOVProfilerPass(Options));
//        if (CodeGenOpts.getDebugInfo() == codegenoptions::NoDebugInfo)
//            MPM->add(createStripSymbolsPass(true));
//    }
    
//    if (CodeGenOpts.hasProfileClangInstr()) {
//        InstrProfOptions Options;
//        Options.NoRedZone = CodeGenOpts.DisableRedZone;
//        Options.InstrProfileOutput = CodeGenOpts.InstrProfileOutput;
//        MPM->add(createInstrProfilingLegacyPass(Options));
//    }
//    if (CodeGenOpts.hasProfileIRInstr()) {
//        if (!CodeGenOpts.InstrProfileOutput.empty())
//            PMBuilder.PGOInstrGen = CodeGenOpts.InstrProfileOutput;
//        else
//            PMBuilder.PGOInstrGen = "default.profraw";
//    }
//    if (CodeGenOpts.hasProfileIRUse())
//        PMBuilder.PGOInstrUse = CodeGenOpts.ProfileInstrumentUsePath;
//    
//    if (!CodeGenOpts.SampleProfileFile.empty()) {
//        MPM->add(createPruneEHPass());
//        MPM->add(createSampleProfileLoaderPass(CodeGenOpts.SampleProfileFile));
//        PMBuilder.addExtension(PassManagerBuilder::EP_EarlyAsPossible,
//                               addCleanupPassesForSampleProfiler);
//    }
    
    managerBuilder.populateFunctionPassManager(*fpm);
    managerBuilder.populateModulePassManager(*mpm);
}

void backend::BackendHelper::runPasses() {
    assert(boundmodule && "No module is bound to the backend helper.");
    
    if (fpm) {
        fpm->doInitialization();
        for (llvm::Function &func : *boundmodule)
            if (func.isDeclaration()) fpm->run(func);
        fpm->doFinalization();
    }
    
    if (mpm) {
        mpm->run(*boundmodule);
    }    
}

bool backend::BackendHelper::emit(fsys::OutputFile *outfile) {
    assert(boundmodule && "No module is bound to the backend helper.");
    assert(target && "No target is bound to the backend helper.");
    
    EmitType etype = EmitNothing;
    bool targetMachineNeeded = false;
    
    llvm::TargetMachine *targetMachine = getTargetInfo().getTargetMachine();
    
    llvm::TargetMachine::CodeGenFileType CGFT = llvm::TargetMachine::CGFT_Null;
    if (outfile) {
        switch (outfile->getType()) {
            case fsys::ObjectCode:
                etype = EmitObjectFile;
                CGFT = llvm::TargetMachine::CGFT_ObjectFile;
                targetMachineNeeded = true;
                break;
            case fsys::TextualAssembly:
                etype = EmitAssemblyFile;
                CGFT = llvm::TargetMachine::CGFT_AssemblyFile;
                targetMachineNeeded = true;
                
                // FIXME this is a temporary fix for a bug which
                // prevents asm from being written to the file
                outfile->os().SetUnbuffered();
                break;
            case fsys::TextualIR:
                etype = EmitIRLLFile;
                break;
            case fsys::BitcodeIR:
                etype = EmitIRBCFile;
                break;
            default:
                llvm_unreachable("Invalid output type.");
                return false;
        }
    }
    
    if (targetMachineNeeded && !targetMachine) return false;
    
    
    llvm::legacy::PassManager *codegenpm = new llvm::legacy::PassManager();
    if (targetMachine) {
        codegenpm->add(llvm::createTargetTransformInfoWrapperPass(getTargetIRAnalysis(targetMachine)));
    }
    
    switch (etype) {
        case EmitNothing: break;
        case EmitIRLLFile: {
            codegenpm->add(llvm::createPrintModulePass(outfile->os())); // FIXME use lists option
            break;
        }
        case EmitIRBCFile: {
            codegenpm->add(llvm::createBitcodeWriterPass(outfile->os())); // FIXME use lists option
            break;
        }
        default: {
            codegenpm->add(new llvm::TargetLibraryInfoWrapperPass(*getTargetInfo().createTLII()));
            
            if (targetMachine->addPassesToEmitFile(*codegenpm, outfile->os(), CGFT))
                return false;
            break;
        }
    }
    
    codegenpm->run(*boundmodule);

    // FIXME this is a temporary fix for a bug that prevents
    // backend output from being written to the files
    outfile->os().flush();
    return true;
}



void backend::init(diag::DiagEngine &diags, opts::BackendOptions &backendOptions, opts::TargetOptions &targetOptions) {
    llvm::PassRegistry *registry = llvm::PassRegistry::getPassRegistry();
    llvm::initializeCore(*registry);
    llvm::initializeCodeGen(*registry);
    llvm::initializeLoopStrengthReducePass(*registry);
    llvm::initializeLowerIntrinsicsPass(*registry);
    llvm::initializeUnreachableBlockElimPass(*registry);
    
    llvm::SmallVector<const char *, 16> llvmargv;
    llvmargv.push_back("hpc"); // Fake name
    llvmargv.push_back(nullptr);
    
    llvm::cl::ParseCommandLineOptions(llvmargv.size() - 1, llvmargv.data());
}
