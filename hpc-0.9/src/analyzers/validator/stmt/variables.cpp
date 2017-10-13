//
//  locals.cpp
//  HumanPlus
//
//  Created by Lore on 09/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/exprs/constant.h>

using namespace hpc;

void validator::ValidatorImpl::visitVar(ast::Var *var) {
    if (ast::Type *type = var->getType()) {
        if (!validate(type))
            var->resignValidation();
    }
}

void validator::ValidatorImpl::visitLocalVar(ast::LocalVar *var) {
    visitVar(var);
    
    if (!getResolver().declareVariable(*var)) {
        var->resignValidation();
    }
    
    if (!var->isValid()) return;
    
    ast::Expr *initVal = var->getInitialValue();
    if (initVal) {
        if (validate(initVal)) {
            ast::Type *initValTy = initVal->evalType();
            
            if (!var->getType() && initValTy->isNullPointerType()) {
                validator.getDiags().reportError(diag::TypeInferenceVariableWithNullLiteral, var->tokenRef(ast::PointToInitValueIntroducer));
                var->resignValidation();
            } else if (!var->getType()) {
                var->setType(initValTy); // type inference
            } else if (var->getType()->isPointerType() && initVal->isNullPointer()) {
                var->setInitialValue(new ast::NullPointer(var->getType()));
            } else if (initValTy->canAssignTo(var->getType())) {
                var->setInitialValue(new ast::ImplicitCastExpr(initVal, var->getType()));
            } else {
                validator.getDiags().reportError(diag::NoViableConversion, var->tokenRef(ast::PointToInitValueIntroducer))
                    << initValTy->asString() << var->getType()->asString();
                var->resignValidation();
            }
        }
    } else if (!var->getType()) {
        validator.getDiags().reportError(diag::TypeInferencedVariableNeedsInitVal, var->tokenRef(ast::PointToVariableIdentifier));
        var->resignValidation();
    }
    
    if (!var->getType() && (!initVal || !initVal->isValid())) {
        var->resignValidation();
    }
}

void validator::ValidatorImpl::visitParamVar(ast::ParamVar *var) {
    visitVar(var);
    
    if (!var->getType() || !var->getType()->isValid() || !getResolver().declareVariable(*var)) {
        var->resignValidation();
    }
    
    // FIXME defaultVal check
}


