// => src/analyzers/validator/exprs/binary.cpp
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
#include <hpc/ast/exprs/binary.h>
#include <hpc/ast/exprs/constant.h>

using namespace hpc;

void validator::ValidatorImpl::visitBinaryExpr(ast::BinaryExpr *expression) {
    ast::Expr *lhs = expression->getLHS();
    ast::Expr *rhs = expression->getRHS();
    
    validate(lhs);
    validate(rhs);
    
    if (!lhs->isValid() || !rhs->isValid()) {
        expression->resignValidation();
        return;
    }
    
    ast::Type *lhsTy = lhs->evalType();
    ast::Type *rhsTy = rhs->evalType();
    
    if (ast::Type::areEquivalent(lhsTy, rhsTy)) {
        expression->setExpressionType(lhsTy->getCanonicalType());
    } else if (rhsTy->canCastTo(lhsTy)) {
        expression->castRHSToType(lhsTy);
        expression->setExpressionType(lhsTy);
    } else if (lhsTy->canCastTo(rhsTy)) {
        expression->castLHSToType(rhsTy);
        expression->setExpressionType(rhsTy);
    } else {
        validator.getDiags().reportError(diag::IncompatibleTypesInBinary, expression->tokenRef(ast::PointToOperator))
            << lhsTy->asString() << rhsTy->asString();
        expression->resignValidation();
    }
}

void validator::ValidatorImpl::visitAssignmentExpr(ast::AssignmentExpr *expression) {
    ast::Expr *lhs = expression->getLHS();
    if (!validate(lhs)) {
        expression->resignValidation();
        return;
    }
    
    ast::Type *lhsTy = lhs->evalType();
    
    expression->expandCompoundAssignment();
    // FIXME this should not be used and compound assignments should be generated directly to allow better optimizations.
    
    ast::Expr *rhs = expression->getRHS();
    if (!validate(rhs)) {
        expression->resignValidation();
        return;
    }
    
    ast::Type *rhsTy = rhs->evalType();
    
    if (!lhs->supportsLeftHandAssignment()) {
        validator.getDiags().reportError(diag::ExpressionNotAssignable, expression->tokenRef(ast::PointToOperator));
        expression->resignValidation();
    } else if (lhsTy->isPointerType() && rhs->isNullPointer()) {
        rhs = new ast::NullPointer(lhsTy);
    } else if (rhsTy->canAssignTo(lhsTy)) {
        expression->castRHSToType(lhsTy);
        //validate(rhs);
        
        expression->setExpressionType(lhsTy);
    } else {
        validator.getDiags().reportError(diag::NoViableConversion, expression->tokenRef(ast::PointToOperator)) << rhsTy->asString() << lhsTy->asString();
        expression->resignValidation();
    }
}

void validator::ValidatorImpl::visitBitwiseExpr(ast::BitwiseExpr *expression) {
    visitBinaryExpr(expression);
    
    if (expression->isValid() && !expression->evalType()->isIntegerType()) {
        validator.getDiags().reportError(diag::IncompatibleTypesInBinary, expression->tokenRef(ast::PointToOperator))
            << expression->getLHS()->evalType()->asString() << expression->getRHS()->evalType()->asString();
        expression->resignValidation();
    }
}
