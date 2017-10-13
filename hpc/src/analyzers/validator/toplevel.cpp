// => src/analyzers/validator/toplevel.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/decls/namespace.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/decls/typealias.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/exprs/constant.h>

using namespace hpc;

void validator::ValidatorImpl::visitNameSpaceDecl(ast::NameSpaceDecl *nameSpace) {
    getResolver().switchTo(nameSpace);
    
    for (ast::Decl *decl : nameSpace->getDeclarations()) {
        if (!validate(decl)) {
            nameSpace->resignValidation();
        }
    }
    
    getResolver().switchToContainer();
}

void validator::ValidatorImpl::visitGlobalVar(ast::GlobalVar *var) {
    visitVar(var);
    
    if (ast::Expr *initVal = var->getInitialValue()) {
        if (validate(initVal)) {
            
            ast::Type *initValTy = initVal->evalType();
            if (!var->getType() && initValTy->isNullPointerType()) {
                validator.getDiags().reportError(diag::TypeInferenceVariableWithNullLiteral, var->tokenRef(ast::PointToInitValueIntroducer));
                var->resignValidation();
            } else if (!var->getType()) {
                var->setType(initValTy); // type inference
            } else if (var->getType()->isPointerType() && initVal->isNullPointer()) {
                initVal = new ast::NullPointer(var->getType());
            } else if (initValTy->canAssignTo(var->getType())) {
                initVal = new ast::ImplicitCastExpr(initVal, var->getType());
            } else {
                validator.getDiags().reportError(diag::NoViableConversion, var->tokenRef(ast::PointToInitValueIntroducer))
                    << initValTy->asString() << var->getType()->asString();
                var->resignValidation();
            }

        } else {
            var->resignValidation();
        }
    } else if (!var->getType()) {
        validator.getDiags().reportError(diag::TypeInferencedVariableNeedsInitVal, var->tokenRef(ast::PointToVariableIdentifier));
        var->resignValidation();
    }
    
    if (ast::GlobalVar *redefVar = getResolver().getInsertNameSpace().getGlobalVariable(ast::Symbol(var->getName()))) {
        if (var != redefVar) { // another variable holds the spot so this is a redefinition.
            if (ast::Type::areEquivalent(redefVar->getType(), var->getType()))
                validator.getDiags().reportError(diag::RedefinitionOfLocalVariable, var->tokenRef(ast::PointToVariableIdentifier))
                    << var->getName();
            else {
                validator.getDiags().reportError(diag::RedefinitionOfLocalVarWithDifType, var->tokenRef(ast::PointToVariableIdentifier))
                    << var->getName() << var->getType()->asString() << redefVar->getType()->asString();
            }
            
            validator.getDiags().reportNote(diag::PreviousVariableDeclarationWasHere, redefVar->tokenRef(ast::PointToVariableIdentifier))
                << redefVar->getName();
            
        }
    }
}

void validator::ValidatorImpl::visitFunctionDecl(ast::FunctionDecl *function) {
    validate(function->getReturnType());
    
    getResolver().openLocalStackForFunction(function);
    getResolver().getInnermostStack().addScope();
    
    if (!function->getReturnType()->isValid()) {
        function->resignValidation();
        return;
    }
    
    for (ast::ParamVar *arg : function->getArgs()) {
        if (!validate(arg)) {
            function->resignValidation();
            break;
        }
    }
    
    if (ast::CompoundStmt *statements = function->getStatementsBlock()) {
        validate(statements);
        
        if (!statements->returns() && !function->getReturnType()->isVoidType()) {
            validator.getDiags().reportError(diag::ControlReachesEndOfNonVoidFunction, function->tokenRef(ast::PointToEndOfFunction));
            function->resignValidation();
        }
        
        if (!statements->isValid()) function->resignValidation();
    }
    
    getResolver().getInnermostStack().clear();
    getResolver().closeLastLocalStack();
    
}

void validator::ValidatorImpl::visitTypeAliasDecl(ast::TypeAliasDecl *alias) {
    
    if (!validate(alias->getOriginalType())) {
        alias->resignValidation();
        return;
    }
    
    if (!validate(alias->getType())) {
        alias->resignValidation();
        return;
    }
    
    if (ast::Type *redefTy = getResolver().getInsertNameSpace().getType(ast::Symbol(alias->getName()))) {
        if (alias->getType() != redefTy) {
            
            if (ast::Type::areEquivalent(alias->getOriginalType(), redefTy)) {
                validator.getDiags().reportError(diag::RedefinitionOfTypeAlias, alias->tokenRef(ast::PointToVariableIdentifier));
            } else {
                validator.getDiags().reportError(diag::RedefinitionOfTypeAliasWithDifType, alias->tokenRef(ast::PointToVariableIdentifier))
                    << alias->getOriginalType()->asString() << redefTy->asString();
            }
            
            validator.getDiags().reportNote(diag::PreviousTypeDeclarationWasHere, redefTy->tokenRef(ast::PointToVariableIdentifier))
                << alias->getName();
            
            alias->resignValidation();
        }
    }
    
}
