//
//  Stmt.cpp
//  Human Plus
//
//  Created by Lore on 20/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/ast/stmt/statement.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/decls/function.h>
#include <hpc/utils/printers.h>
#include <hpc/analyzers/validator/validator.h>
#include <hpc/diagnostics/diagnostics.h>

using namespace hpc;

void ast::CompoundStmt::pushSubStatement(Stmt *statement) {
    substatements.push_back(statement);
}

bool ast::CompoundStmt::returns() const {
    bool rets = false;
    for (Stmt *stmt : substatements) {
        if (rets) {
            //VS.diags.reportError(diag::InstructionsNeverExecuted);
            return true;
        }
        rets = stmt->returns();
    }
    
    return rets;
}

int ast::CompoundStmt::containedReturns() const {
    int rets = 0;
    for (Stmt *stmt : substatements)
        rets += stmt->containedReturns();
    
    return rets;
}


ast::VarDeclStmt::VarDeclStmt(FunctionDecl *container) : container(container) {  }

void ast::VarDeclStmt::addVariable(Var *var) {
    variables.push_back(var);
}

ast::ReturnStmt::ReturnStmt(FunctionDecl *container, Expr *returnVal) : container(container), returnVal(returnVal) {  }

void ast::ReturnStmt::castReturnValueToType(ast::Type *destination) {
    if (destination->isBooleanType()) {
        returnVal = new ast::EvalExpr(returnVal);
    } else {
        returnVal = new ast::ImplicitCastExpr(returnVal, destination);
    }
}

ast::BreakStmt::BreakStmt() {  }

ast::ContinueStmt::ContinueStmt() {  }
