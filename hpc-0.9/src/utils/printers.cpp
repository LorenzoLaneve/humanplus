//
//  printers.cpp
//  Human Plus
//
//  Created by Lore on 09/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/utils/printers.h>

using namespace hpc;

void util::printEscapedChar(char c) {
    switch(c){
        case '\\':
            llvm::outs() << "\\\\";
            break;
        case '\?':
            llvm::outs() << "\\?";
            break;
        case '\0':
            llvm::outs() << "\\0";
            break;
        case '\a':
            llvm::outs() << "\\a";
            break;
        case '\b':
            llvm::outs() << "\\b";
            break;
        case '\f':
            llvm::outs() << "\\f";
            break;
        case '\n':
            llvm::outs() << "\\n";
            break;
        case '\r':
            llvm::outs() << "\\r";
            break;
        case '\t':
            llvm::outs() << "\\t";
            break;
        case '\v':
            llvm::outs() << "\\v";
            break;
        default:
            llvm::outs() << c;
    }
}

void util::printEscapedString(std::string s) {
    for (char &c : s) printEscapedChar(c);
}
