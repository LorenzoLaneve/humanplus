//
//  toplevel.cpp
//  HumanPlus
//
//  Created by Lore on 09/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/extras/dump/dump.h>

using namespace hpc;

void extras::NewASTPrinter::visitNameSpaceDecl(ast::NameSpaceDecl *nameSpace) {
    printObject("NameSpaceDecl", nameSpace);
    os << nameSpace->getName() << "\n";
    
    openChildBranch(nameSpace->getDeclarations().size());
    for (ast::Decl *decl : nameSpace->getDeclarations())
        takeDecl(decl);
    
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitFunctionDecl(ast::FunctionDecl *function) {
    printObject("FunctionDecl", function);
    os << function->getName() << " ";
    
    takeType(function->getReturnType());
    os << "\n";
    
    openChildBranch(function->getArgs().size()+1);
    
    for (ast::ParamVar *param : function->getArgs())
        takeDecl(param);
    
    if (ast::Stmt *statements = function->getStatementsBlock()) {
        takeStmt(statements);
    } else {
        printBranch();
        os << "(external function)\n";
    }
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitEnumDecl(ast::EnumDecl *enumDecl) {
    printObject("EnumDecl", enumDecl);
    //os << enumDecl->getName() << "\n";
}

void extras::NewASTPrinter::visitClassDecl(ast::ClassDecl *classDecl) {
    printObject("ClassDecl", classDecl);
    os << classDecl->getName() << "\n";
    
    
    openChildBranch(classDecl->getMembers().size());
    
    for (ast::Decl *member : classDecl->getMembers()) {
        takeDecl(member);
    }
    
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitProtocolDecl(ast::ProtocolDecl *protocolDecl) {
    printObject("ProtocolDecl", protocolDecl);
    os << protocolDecl->getName() << "\n";
    
    // protocol members dumping
}

void extras::NewASTPrinter::visitTypeAliasDecl(ast::TypeAliasDecl *alias) {
    printObject("TypeAliasDecl", alias);
    os << alias->getName() << " ";
    
    takeType(alias->getOriginalType());
    
    os << "\n";
}

