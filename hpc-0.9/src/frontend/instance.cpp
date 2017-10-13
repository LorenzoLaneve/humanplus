//
//  compiler.cpp
//  HumanPlus
//
//  Created by Lore on 09/01/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#include <hpc/frontend/instance.h>
#include <hpc/frontend/invocation.h>

using namespace hpc;

CompilerInstance::~CompilerInstance()  {
    delete invocation;
    delete diags;
    delete printer;
}

void CompilerInstance::createDiagnostics() {
    assert(invocation && "The compiler instance has not been invoked.");
    
    printer = new diag::DiagPrinter(llvm::errs(), invocation->getDiagOptions());
    
    diags = new diag::DiagEngine(invocation->getDiagOptions(), *printer);
}

bool CompilerInstance::canHandleFile(fsys::File &file) {
    switch (file.getType()) {
        case fsys::SourceFile:
        case fsys::HumanPlusKit:
        case fsys::TextualIR:
        case fsys::BitcodeIR:
            return true;
        default:
            return false;
    }
}
