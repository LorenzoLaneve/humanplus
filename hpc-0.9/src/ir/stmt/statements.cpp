//
//  statements.cpp
//  HumanPlus
//
//  Created by Lore on 11/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ir/builders.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/decls/function.h>
#include <hpc/ast/stmt/statement.h>

#include <llvm/IR/Value.h>

using namespace hpc;

void codegen::ModuleBuilder::visitCompoundStmt(ast::CompoundStmt *statement) {
    for (ast::Stmt *substmt : statement->statements())
        takeStmt(substmt);
}

void codegen::ModuleBuilder::visitVarDeclStmt(ast::VarDeclStmt *statement) {
    for (ast::Var *variable : statement->getDeclaredVariables()) {
        // TODO stack objects initializations
        
        if (ast::Expr *initVal = variable->getInitialValue()) assign(variable, build(initVal));
    }
}

void codegen::ModuleBuilder::visitReturnStmt(ast::ReturnStmt *statement) {
    ast::FunctionDecl *receiver = statement->getReceiver();
    
    if (receiver->containedReturns() > 1) {
        if (!receiver->getReturnType()->isVoidType()) {
            if (llvm::Value *reg = returnRegister) // FIXME a table keeping links between functions and return registers?
                builder->CreateStore(build(statement->getReturnValue()), reg);
        }
        
        table.setValForComponent(statement, builder->CreateBr(returnBlock));
        return;
    }
    
    if (!receiver->getReturnType()->isVoidType())
        table.setValForComponent(statement, builder->CreateRet(build(statement->getReturnValue())));
    else table.setValForComponent(statement, builder->CreateRetVoid());
}

void codegen::ModuleBuilder::visitBreakStmt(ast::BreakStmt *statement) {
    table.makeBreak(statement->getBreakCatcher());
}

void codegen::ModuleBuilder::visitContinueStmt(ast::ContinueStmt *statement) {
    table.makeContinue(statement->getContinueCatcher());
}
