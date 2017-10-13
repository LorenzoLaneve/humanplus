// => src/target/target.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/target/target.h>
#include <hpc/utils/opts.h>

#include <llvm/ADT/StringExtras.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/MC/SubtargetFeature.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetSubtargetInfo.h>

using namespace hpc;

#define DisableFPElim false                             // FIXME: -disable-fp-elim flag
#define DisableTailCalls false                          // FIXME: -disable-tail-calls flag
#define StackRealign false                              // FIXME: -stackrealign flag
#define TrapFuncName ""                                 // FIXME: -trap-func flag

#define FRelocModel llvm::Reloc::Default                // FIXME: -relocation-model by flag
#define FCMModel llvm::CodeModel::Default               // FIXME: -code-model by flag
#define NoIntegratedAssembler false                     // FIXME: -no-integrated-as by flag
#define FShowMCEncoding false                           // FIXME: -show-mc-encoding by flag
#define EnableDwarfDirectory false                      // FIXME: -enable-dwarf-directory by flag
#define FAsmVerbose true                                // FIXME: -asm-verbose by flag
#define DisableSimplifyLibCalls false                   // FIXME: -disable-simplify-libcalls by flag


target::TargetInfo::TargetInfo(opts::TargetOptions &targetOptions)
: targetOptions(targetOptions), triple(targetOptions.targetTriple) {

    CPU = targetOptions.CPU;
    
    llvm::StringMap<bool> features;
    for (std::string feature : targetOptions.features) {
        features[feature] = true;
    }
    // FIXME features are missing.

}

bool target::TargetInfo::createTargetABI() {
    theABI = TargetABI::createABI(targetOptions);
    return true;
}

llvm::TargetLibraryInfoImpl *target::TargetInfo::createTLII() {
    llvm::TargetLibraryInfoImpl *TLII = new llvm::TargetLibraryInfoImpl(llvm::Triple(triple));
    
    if (DisableSimplifyLibCalls)
        TLII->disableAllFunctions();
    
    return TLII;
}

void target::TargetInfo::addTargetAttribs(modules::ModuleWrapper &wrapper) {

    for (llvm::Function &func : wrapper.getModule()) {
        llvm::LLVMContext &context = func.getContext();
        llvm::AttributeSet attribs = func.getAttributes(), nattribs;
        
        if (!CPU.empty())
            nattribs = nattribs.addAttribute(context, llvm::AttributeSet::FunctionIndex,
                                             "target-cpu", CPU);
        
        if (!featureString.empty())
            nattribs = nattribs.addAttribute(context, llvm::AttributeSet::FunctionIndex,
                                             "target-features", featureString);
        
        if (DisableFPElim)
            nattribs = nattribs.addAttribute(context, llvm::AttributeSet::FunctionIndex,
                                             "no-frame-pointer-elim", llvm::toStringRef(DisableFPElim));
        
        if (DisableTailCalls)
            nattribs = nattribs.addAttribute(context, llvm::AttributeSet::FunctionIndex,
                                             "disable-tail-calls", llvm::toStringRef(DisableTailCalls));
        
        if (StackRealign)
            nattribs = nattribs.addAttribute(context, llvm::AttributeSet::FunctionIndex,
                                             "stackrealign");
        
        if (/* DISABLES CODE */ (false)) // FIXME
            for (auto &block : func) for (auto &I : block)
                if (auto *fcall = llvm::dyn_cast<llvm::CallInst>(&I))
                    if (const auto *F = fcall->getCalledFunction())
                        if (F->getIntrinsicID() == llvm::Intrinsic::debugtrap ||
                            F->getIntrinsicID() == llvm::Intrinsic::trap)
                            fcall->addAttribute(llvm::AttributeSet::FunctionIndex,
                                                "trap-func-name", TrapFuncName);
        
        nattribs = attribs.addAttributes(context, llvm::AttributeSet::FunctionIndex, nattribs);
        func.setAttributes(nattribs);
    }
}

bool target::TargetInfo::createTargetMachine(opts::BackendOptions &backendOptions, diag::DiagEngine &diags) {
    
    std::string featureString = llvm::join(targetOptions.features.begin(), targetOptions.features.end(), ",");
    
    llvm::Triple ttriple(targetOptions.targetTriple);
    
    std::string errstring;
    const llvm::Target *targetlu = llvm::TargetRegistry::lookupTarget("", ttriple, errstring);
    if (!targetlu) {
        diags.reportError(diag::UnableToCreateLLVMTarget) << errstring;
        targetMachine = nullptr;
        return false;
    }
    
    llvm::CodeGenOpt::Level optimization = llvm::CodeGenOpt::Default;
    switch (backendOptions.optimizationLevel) {
        case 0: optimization = llvm::CodeGenOpt::None; break;
        case 3: optimization = llvm::CodeGenOpt::Aggressive; break;
    }
    
    llvm::TargetOptions topts;
    topts.DisableIntegratedAS = NoIntegratedAssembler;
    topts.MCOptions.ShowMCEncoding = FShowMCEncoding;
    topts.MCOptions.MCUseDwarfDirectory = EnableDwarfDirectory;
    topts.MCOptions.AsmVerbose = FAsmVerbose;
    
    targetMachine = targetlu->createTargetMachine(ttriple.getTriple(), targetOptions.CPU, featureString, topts, FRelocModel, FCMModel, optimization);
    
    return true;
}

llvm::DataLayout &target::TargetInfo::getDataLayout() {
    if (!datalayout) {
        datalayout = new llvm::DataLayout(getTargetMachine()->createDataLayout());
    }
    return *datalayout;
}

target::TargetInfo *target::TargetInfo::fromOptions(opts::TargetOptions &targetOpts, diag::DiagEngine &diags) {
    return new target::TargetInfo(targetOpts);
}
