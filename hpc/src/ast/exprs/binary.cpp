// => src/ast/exprs/binary.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/exprs/binary.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/types/builtintype.h>

using namespace hpc;

ast::BinaryExpr::BinaryExpr(Expr *lhs, lexer::token_ty oper, Expr *rhs) : lhs(lhs), oper(oper), rhs(rhs) {  }

void ast::BinaryExpr::castLHSToType(ast::Type *destination) {
    if (destination->isBooleanType()) {
        lhs = new ast::EvalExpr(lhs);
    } else {
        lhs = new ast::ImplicitCastExpr(lhs, destination);
    }
}

void ast::BinaryExpr::castRHSToType(ast::Type *destination) {
    if (destination->isBooleanType()) {
        rhs = new ast::EvalExpr(rhs);
    } else {
        rhs = new ast::ImplicitCastExpr(rhs, destination);
    }
}

void ast::BinaryExpr::expandCompoundAssignment() {
    if (OP_IS_COMPOUND_ASSIGNMENT(oper)) {
        rhs = BinaryExpr::create(lhs, OP_DETACH_ASSIGNMENT(oper), rhs);
        oper = lexer::TokenOperatorAssign;
    }
}

ast::Expr *ast::BinaryExpr::create(Expr *lhs, lexer::token_ty oper, Expr *rhs) {
    switch (oper) {
        case lexer::TokenOperatorPlus:
        case lexer::TokenOperatorMinus:
        case lexer::TokenOperatorMultiply:
        case lexer::TokenOperatorDivide:
        case lexer::TokenOperatorRemainder:
            lhs = new ast::ArithmeticExpr(lhs, oper, rhs);
            break;
            
        case lexer::TokenOperatorLower:
        case lexer::TokenOperatorLowerEqual:
        case lexer::TokenOperatorGreater:
        case lexer::TokenOperatorGreaterEqual:
        case lexer::TokenOperatorEqual:
        case lexer::TokenOperatorNotEqual:
            lhs = new ast::ComparisonExpr(lhs, oper, rhs);
            break;
            
        case lexer::TokenOperatorAssign:
        case lexer::TokenOperatorPlusEqual:
        case lexer::TokenOperatorMinusEqual:
        case lexer::TokenOperatorMultiplyEqual:
        case lexer::TokenOperatorDivideEqual:
        case lexer::TokenOperatorRemainderEqual:
            lhs = new ast::AssignmentExpr(lhs, oper, rhs);
            break;
            
        case lexer::TokenOperatorLeftShift:
        case lexer::TokenOperatorRightShift:
        case lexer::TokenOperatorBitwiseAnd:
        case lexer::TokenOperatorBitwiseOr:
            lhs = new ast::BitwiseExpr(lhs, oper, rhs);
            break;
            
        default:
            break;
    }
    
    return lhs;
}


ast::Type *ast::ArithmeticExpr::evalType() {
    return negotiatedType;
}

ast::Type *ast::ComparisonExpr::evalType() {
    return BuiltinType::get(BuiltinType::Boolean);
}

ast::Type *ast::AssignmentExpr::evalType() {
    return lhs->evalType();
}

ast::Type *ast::BitwiseExpr::evalType() {
    return negotiatedType;
}
