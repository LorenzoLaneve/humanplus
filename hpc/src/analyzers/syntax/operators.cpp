// => src/analyzers/syntax/operators.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/syntax/operators.h>
#include <hpc/analyzers/lexer/tokens.h>

#include <assert.h>
#include <map>

using namespace hpc;


syntax::OperatorPrecedence syntax::getOperatorPrecedence(OperatorToken op) {
    switch (op) {
#define __bin_operator_def(id, toString, tokenValue, precedence, associativity) case lexer::Token::id: return precedence;
#define __un_operator_def(id, toString, tokenValue)
#include <hpc/inc/syntax/operators.inc>
            
#undef __bin_operator_def
#undef __un_operator_def
        default:
            llvm_unreachable("The given token is not a binary operator.");
    }
}

syntax::OperatorAssociativity syntax::getOperatorAssociativity(OperatorToken op) {
    switch (op) {
#define __bin_operator_def(id, toString, tokenValue, precedence, associativity) case lexer::Token::id: return Assoc##associativity;
#define __un_operator_def(id, toString, tokenValue)
#include <hpc/inc/syntax/operators.inc>
            
#undef __bin_operator_def
#undef __un_operator_def
        default:
            llvm_unreachable("The given token is not a binary operator.");
    }
}

bool syntax::isBinaryOperator(OperatorToken op) {
    switch (op) {
#define __bin_operator_def(id, toString, tokenValue, precedence, associativity) case lexer::Token::id:
#define __un_operator_def(id, toString, tokenValue)
#include <hpc/inc/syntax/operators.inc>
            
#undef __bin_operator_def
#undef __un_operator_def
            return true;
        default:
            return false;
    }
}

bool syntax::isUnaryOperator(OperatorToken op) {
    switch (op) {
#define __bin_operator_def(id, toString, tokenValue, precedence, associativity)
#define __un_operator_def(id, toString, tokenValue) case lexer::Token::id:
#include <hpc/inc/syntax/operators.inc>
            
#undef __bin_operator_def
#undef __un_operator_def
            return true;
        default:
            return false;
    }
}

