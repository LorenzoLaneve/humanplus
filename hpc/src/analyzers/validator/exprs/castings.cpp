// => src/analyzers/validator/exprs/castings.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/exprs/constant.h>
#include <hpc/config.h>

using namespace hpc;

void validator::ValidatorImpl::visitImplicitCastExpr(ast::ImplicitCastExpr *cast) {
    if (!cast->getExpression()->evalType()->canCastTo(cast->getDestination())) {
        cast->resignValidation();
    }
}

void validator::ValidatorImpl::visitEvalExpr(ast::EvalExpr *cast) {
    if (!validate(cast->getExpression())) {
        cast->resignValidation();
        return;
    }
    
    ast::Type *valTy = cast->getExpression()->evalType();
    
    switch (valTy->getFormat()) {
#ifdef __human_plus_compiler_supports_bool_context_conversion
        case ast::TypeFormatPointer:
        case ast::TypeFormatFloatingPoint:
#endif
        case ast::TypeFormatInteger:
        case ast::TypeFormatUnsignedInteger:
        case ast::TypeFormatSignedInteger:
        case ast::TypeFormatBoolean:
            break;
        default:
            validator.getDiags().reportError(diag::ExpressionIsNotEvaluable, cast->getExpression()->completeRef())
                << valTy->asString();
            cast->resignValidation();
            return;
    }
    
}
