//
//  hpc.cpp
//  HumanPlus
//
//  Created by Lore on 09/01/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#include <hpc/frontend/instance.h>
#include <hpc/frontend/invocation.h>

#include <hpc/utils/printers.h>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Signals.h>
#include <llvm/Option/OptTable.h>
#include <llvm/Option/ArgList.h>

using namespace hpc;

static void handle_llvm_error(void *userdata, const std::string &message, bool generateCrashDiags) {
    diag::DiagEngine &diags = *static_cast<diag::DiagEngine *>(userdata);
    
    diags.reportDiag(diag::Error, diag::LLVMFatalError) << message;
    
    llvm::sys::RunInterruptHandlers();
    
    // An exit code 70 will generate crash diags as requested by LLVM.
    exit(generateCrashDiags ? 70 : 1);
}

extern "C" int hpc_main(const char **argBegin, const char **argEnd, const char *argv0, void *main) {
    llvm::llvm_shutdown_obj X;
    
    CompilerInstance hpc;
    
    // Initialize LLVM targets before starting so they can be used by --version
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();
    
    // Empty diagnostics options for the temporary engine.
    opts::DiagnosticsOptions emptyopts;
    
    
    diag::DiagBufferer *diagBuffer = new diag::DiagBufferer();
    
    // Temporary engine that will take any errors for args parsing.
    diag::DiagEngine *argDiagEngine = new diag::DiagEngine(emptyopts, *diagBuffer);
    
    bool success = CompilerInvocation::create(hpc, argBegin, argEnd, *argDiagEngine);
    
    // The actual diagnostics engine that will be used.
    hpc.createDiagnostics();
    if (!hpc.hasDiagnostics()) {
        return 1;
    }
    
    // Let's get LLVM errors to be printed by hpc diagnostics engine.
    llvm::install_fatal_error_handler(handle_llvm_error, static_cast<void *>(&hpc.getDiagnostics()));
    
    // Report all the diagnostics reported by the temporary diagnostics engine to the actual engine.
    diagBuffer->passToEngine(hpc.getDiagnostics());
    
    // let's get rid of the temporary diag engine.
    delete argDiagEngine;
    delete diagBuffer;
    
    if (!success)
        return 1;
    
    success = hpc.executeInvocation();
    
    // Done, let's remove the LLVM fatal error handler before closing.
    llvm::remove_fatal_error_handler();
    
    return !success;
}
