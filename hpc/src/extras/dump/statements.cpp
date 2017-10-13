// => src/extras/dump/statements.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/extras/dump/dump.h>

using namespace hpc;

void extras::NewASTPrinter::visitCompoundStmt(ast::CompoundStmt *statement) {
    printObject("CompoundStmt", statement);
    os << "\n";
    
    openChildBranch(statement->statements().size());

    for (ast::Stmt *substmt : statement->statements()) {
        takeStmt(substmt);
    }
    
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitVarDeclStmt(ast::VarDeclStmt *statement) {
    printObject("VarDeclStmt", statement);
    os << "\n";
    
    openChildBranch(statement->getDeclaredVariables().size());
    
    for (ast::Var *lvar : statement->getDeclaredVariables()) {
        takeDecl(lvar);
    }
    
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitReturnStmt(ast::ReturnStmt *statement) {
    printObject("ReturnStmt", statement);
    os << "\n";
    
    if (ast::Expr *retVal = statement->getReturnValue()) {
        openChildBranch(1);
        takeStmt(retVal);
        closeLastChildBranch();
    }
}

void extras::NewASTPrinter::visitBreakStmt(ast::BreakStmt *statement) {
    printObject("BreakStmt", statement);
    os << "\n";
}

void extras::NewASTPrinter::visitContinueStmt(ast::ContinueStmt *statement) {
    printObject("ContinueStmt", statement);
    os << "\n";
}

void extras::NewASTPrinter::visitIfStmt(ast::IfStmt *statement) {
    printObject("IfStmt", statement);
    os << "\n";
    
    openChildBranch(3);
    takeStmt(statement->getCondition());
    
    if (ast::Stmt *thenBlock = statement->getThenBlock()) {
        takeStmt(thenBlock);
    } else {
        printBranch();
        llvm::outs() << "(no then block)\n";
    }
    
    if (ast::Stmt *elseBlock = statement->getElseBlock()) {
        takeStmt(elseBlock);
    } else {
        printBranch();
        llvm::outs() << "(no else block)\n";
    }
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitSimpleIterStmt(ast::SimpleIterStmt *statement) {
    os << "\n";
    
    openChildBranch(2);
    takeStmt(statement->getCondition());
    
    if (ast::Stmt *block = statement->getBlock()) {
        takeStmt(block);
    } else {
        printBranch();
        os << "(no block)\n";
    }
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitPreWhileStmt(ast::PreWhileStmt *statement) {
    printObject("PreWhileStmt", statement);
    visitSimpleIterStmt(statement);
}

void extras::NewASTPrinter::visitPreUntilStmt(ast::PreUntilStmt *statement) {
    printObject("PreUntilStmt", statement);
    visitSimpleIterStmt(statement);
}

void extras::NewASTPrinter::visitPostWhileStmt(ast::PostWhileStmt *statement) {
    printObject("PostWhileStmt", statement);
    visitSimpleIterStmt(statement);
}

void extras::NewASTPrinter::visitPostUntilStmt(ast::PostUntilStmt *statement) {
    printObject("PostUntilStmt", statement);
    visitSimpleIterStmt(statement);
}

void extras::NewASTPrinter::visitForStmt(ast::ForStmt *statement) {
    printObject("ForStmt", statement);
    os << "\n";
    
    openChildBranch(statement->getInitStatements().size() + statement->getEndStatements().size() + 2);
    
    for (ast::Stmt *stmt : statement->getInitStatements()) {
        takeStmt(stmt);
    }
    
    takeStmt(statement);
    
    for (ast::Stmt *stmt : statement->getEndStatements()) {
        takeStmt(stmt);
    }
    
    if (ast::Stmt *block = statement->getBlock()) {
        takeStmt(block);
    } else {
        printBranch();
        os << "(no block)\n";
    }
    
    closeLastChildBranch();
}

