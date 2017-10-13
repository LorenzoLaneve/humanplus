// => src/extras/dump/dump.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/extras/dump/dump.h>

using namespace hpc;

void extras::dumpAST(ast::AbstractSyntaxTree *ast) {
    assert(ast && "AST cannot be null.");
    
    NewASTPrinter printer;
    printer.print(*ast);
}

void extras::NewASTPrinter::print(ast::AbstractSyntaxTree &theAST) {
    if (os.has_colors()) os.changeColor(llvm::raw_ostream::SAVEDCOLOR, true);
    os << "AbstractSyntaxTree ";
    if (os.has_colors()) os.changeColor(llvm::raw_ostream::YELLOW);
    os << &theAST << "\n";
    if (os.has_colors()) os.resetColor();
    
    openChildBranch(theAST.getRootNameSpace()->getDeclarations().size());
    
    for (ast::Decl *decl : theAST.getRootNameSpace()->getDeclarations())
        takeDecl(decl);
    
    closeLastChildBranch();
}

void extras::NewASTPrinter::openChildBranch(unsigned long children) {
    branches.push_back(children);
}

void extras::NewASTPrinter::closeLastChildBranch() {
    if (branches.size()) branches.pop_back();
}

void extras::NewASTPrinter::printBranch() {
    int i = 0;
    
    if (os.has_colors()) os.changeColor(llvm::raw_ostream::BLUE, true);
    for (unsigned long &branchCount : branches) {
        if (++i == branches.size()) {
            if (branchCount --> 1) {
                os << "|-";
            } else {
                os << "`-";
            }
        } else {
            if (branchCount > 0) {
                os << "| ";
            } else {
                os << "  ";
            }
        }
    }
    if (os.has_colors()) os.resetColor();
}


