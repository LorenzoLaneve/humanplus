//
//  precedence.cpp
//  Human Plus
//
//  Created by Lore on 10/03/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/utils/printers.h>
#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/analyzers/syntax/operators.h>

#include <map>

#define __hp_precedence_exponentiative  160
#define __hp_precedence_multiplicative  150
#define __hp_precedence_additive        140
#define __hp_precedence_comparative     130
#define __hp_precedence_logical_and     120
#define __hp_precedence_logical_or      110
#define __hp_precedence_assignment      90

using namespace hpc;

static std::map<lexer::token_ty, int> precedences = {
    { lexer::TokenOperatorLeftShift,                                  __hp_precedence_exponentiative },
    { lexer::TokenOperatorRightShift,                                 __hp_precedence_exponentiative },
    { lexer::TokenOperatorMultiply,                                   __hp_precedence_multiplicative },
    { lexer::TokenOperatorDivide,                                     __hp_precedence_multiplicative },
    { lexer::TokenOperatorRemainder,                                  __hp_precedence_multiplicative },
    { lexer::TokenOperatorBitwiseAnd,                                 __hp_precedence_multiplicative },
    { lexer::TokenOperatorPlus,                                       __hp_precedence_additive },
    { lexer::TokenOperatorMinus,                                      __hp_precedence_additive },
    { lexer::TokenOperatorBitwiseOr,                                  __hp_precedence_additive },
    { lexer::TokenOperatorLower,                                      __hp_precedence_comparative },
    { lexer::TokenOperatorGreater,                                    __hp_precedence_comparative },
    { lexer::TokenOperatorLowerEqual,                                 __hp_precedence_comparative },
    { lexer::TokenOperatorGreaterEqual,                               __hp_precedence_comparative },
    { lexer::TokenOperatorEqual,                                      __hp_precedence_comparative },
    { lexer::TokenOperatorNotEqual,                                   __hp_precedence_comparative },
    { lexer::TokenOperatorLogicalAnd,                                 __hp_precedence_logical_and },
    { lexer::TokenOperatorLogicalOr,                                  __hp_precedence_logical_or },
    { lexer::TokenOperatorAssign,                                     __hp_precedence_assignment },
    { lexer::TokenOperatorMultiplyEqual,                              __hp_precedence_assignment },
    { lexer::TokenOperatorDivideEqual,                                __hp_precedence_assignment },
    { lexer::TokenOperatorRemainderEqual,                             __hp_precedence_assignment },
    { lexer::TokenOperatorPlusEqual,                                  __hp_precedence_assignment },
    { lexer::TokenOperatorMinusEqual,                                 __hp_precedence_assignment }
};

void util::printOperator(lexer::token_ty oper) {
    assert(syntax::isOperator(oper) && "oper is not a valid Human Plus operator");
    
    if (__operator_is_compound_assignment(oper)) {
        printOperator(__operator_detach_assignment(oper));
        llvm::outs() << "=";
    } else
        
        switch (oper) {
            case lexer::TokenOperatorLowerEqual:
                llvm::outs() << "<=";
                break;
            case lexer::TokenOperatorGreaterEqual:
                llvm::outs() << ">=";
                break;
            case lexer::TokenOperatorEqual:
                llvm::outs() << "==";
                break;
            case lexer::TokenOperatorNotEqual:
                llvm::outs() << "!=";
                break;
            case lexer::TokenOperatorLogicalAnd:
                llvm::outs() << "&&";
                break;
            case lexer::TokenOperatorLogicalOr:
                llvm::outs() << "||";
                break;
            case lexer::TokenOperatorLeftShift:
                llvm::outs() << "<<";
                break;
            case lexer::TokenOperatorRightShift:
                llvm::outs() << ">>";
                break;
            default:
                llvm::outs() << (char)oper;
                break;
        }
    
}

int syntax::getOperatorPrecedence(lexer::token_ty token) {
    return precedences[token];
}

bool syntax::isUnaryOperator(lexer::token_ty token) {
    switch (token) {
        case lexer::TokenOperatorExclMark:
        case lexer::TokenOperatorPlus:
        case lexer::TokenOperatorMinus:
        case lexer::TokenOperatorTilde:
            return true;
        default:
            return false;
    }
}

bool syntax::isBinaryOperator(lexer::token_ty token) {
    switch (token) {
        case lexer::TokenOperatorMultiply:
        case lexer::TokenOperatorDivide:
        case lexer::TokenOperatorRemainder:
        case lexer::TokenOperatorPlus:
        case lexer::TokenOperatorMinus:
        case lexer::TokenOperatorLower:
        case lexer::TokenOperatorGreater:
        case lexer::TokenOperatorLowerEqual:
        case lexer::TokenOperatorGreaterEqual:
        case lexer::TokenOperatorEqual:
        case lexer::TokenOperatorNotEqual:
        case lexer::TokenOperatorAssign:
        case lexer::TokenOperatorMultiplyEqual:
        case lexer::TokenOperatorDivideEqual:
        case lexer::TokenOperatorRemainderEqual:
        case lexer::TokenOperatorPlusEqual:
        case lexer::TokenOperatorMinusEqual:
        case lexer::TokenOperatorLogicalAnd:
        case lexer::TokenOperatorLogicalOr:
        case lexer::TokenOperatorLeftShift:
        case lexer::TokenOperatorRightShift:
        case lexer::TokenOperatorBitwiseAnd:
        case lexer::TokenOperatorBitwiseOr:
            return true;
        default:
            return false;
    }
}

bool syntax::isOperator(lexer::token_ty token) {
    return isBinaryOperator(token) || isUnaryOperator(token);
}
