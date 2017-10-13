//
//  statements.cpp
//  HumanPlus
//
//  Created by Lore on 09/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/stmt/statement.h>

using namespace hpc;

void validator::ValidatorImpl::visitCompoundStmt(ast::CompoundStmt *statement) {
    getResolver().getInnermostStack().addScope();
    for (ast::Stmt *statement : statement->statements()) {
        if (!validate(statement))
            statement->resignValidation();
    }
    getResolver().getInnermostStack().removeScope();
}

void validator::ValidatorImpl::visitVarDeclStmt(ast::VarDeclStmt *statement) {
    for (ast::Var *variable : statement->getDeclaredVariables()) {
        statement->getContainerFunction()->addLocalDeclaration(variable);
        if (!validate(variable)) {
            statement->resignValidation();
        }
    }
}

void validator::ValidatorImpl::visitReturnStmt(ast::ReturnStmt *statement) {
    ast::Expr *returnVal = statement->getReturnValue();
    
    if (returnVal && !validate(returnVal)) {
            statement->resignValidation();
            return;
    }
    
    ast::Type *returnType = statement->getReceiver()->getReturnType();
    
    if (returnVal && returnType->isVoidType()) {
        validator.getDiags().reportError(diag::ReturnForVoidFunctionHasValue, statement->tokenRef(ast::PointToStatementQualifier))
            << statement->getReceiver()->getName();
        statement->resignValidation();
        return;
    } else if (!returnVal) {
        validator.getDiags().reportError(diag::ReturnForNonVoidFunctionNeedsValue, statement->tokenRef(ast::PointToStatementQualifier))
            << statement->getReceiver()->getName();
        statement->resignValidation();
        return;
    } else if (returnType->isVoidType()) {
        return;
    }
    
    ast::Type *returnValTy = returnVal->evalType();
    
    if (!ast::Type::areEquivalent(returnValTy, returnType)) {
        if (returnValTy->canAssignTo(returnType)) {
            statement->castReturnValueToType(returnType);
        } else {
            validator.getDiags().reportError(diag::NoViableConversionInReturn, statement->tokenRef(ast::PointToStatementQualifier))
                << returnValTy->asString() << returnType->asString();
            statement->resignValidation();
        }
    }
}

void validator::ValidatorImpl::visitBreakStmt(ast::BreakStmt *statement) {
    if (ast::Stmt *catcher = getResolver().getInnermostStack().getFirstBreakCatcher()) {
        statement->setBreakCatcher(catcher);
    } else {
        validator.getDiags().reportError(diag::BreakNotInBreakableStatement, statement->tokenRef(ast::PointToStatementQualifier));
        statement->resignValidation();
    }
}

void validator::ValidatorImpl::visitContinueStmt(ast::ContinueStmt *statement) {
    if (ast::Stmt *receiver = getResolver().getInnermostStack().getFirstContinueCatcher()) {
        statement->setContinueCatcher(receiver);
    } else {
        validator.getDiags().reportError(diag::BreakNotInBreakableStatement, statement->tokenRef(ast::PointToStatementQualifier));
        statement->resignValidation();
    }
}

