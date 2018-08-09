// => hpc/inc/syntax/operators.inc
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __bin_operator_def
#error "__bin_operator_def not defined before including operators.inc"
#endif

#ifndef __un_operator_def
#error "__un_operator_def not defined before including operators.inc"
#endif

//__bin_operator_def(id, toString, tokenValue, precedence, associativity)
//__un_operator_def(id, toString, tokenValue)


__bin_operator_def(OperatorLeftShift,       "<<",   -300,                                       prec::Exponentiative, LeftToRight)
__bin_operator_def(OperatorRightShift,      ">>",   -301,                                       prec::Exponentiative, LeftToRight)
__bin_operator_def(OperatorMultiply,        "*",    '*',                                        prec::Multiplicative, LeftToRight)
__bin_operator_def(OperatorDivide,          "/",    '/',                                        prec::Multiplicative, LeftToRight)
__bin_operator_def(OperatorRemainder,       "%",    '%',                                        prec::Multiplicative, LeftToRight)
__bin_operator_def(OperatorBitwiseAnd,      "&",    '&',                                        prec::Multiplicative, LeftToRight)
__bin_operator_def(OperatorPlus,            "+",    '+',                                        prec::Additive,       LeftToRight)
__bin_operator_def(OperatorMinus,           "-",    '-',                                        prec::Additive,       LeftToRight)
__bin_operator_def(OperatorBitwiseOr,       "|",    '|',                                        prec::Additive,       LeftToRight)
__bin_operator_def(OperatorLower,           "<",    '<',                                        prec::Comparative,    LeftToRight)
__bin_operator_def(OperatorGreater,         ">",    '>',                                        prec::Comparative,    LeftToRight)
__bin_operator_def(OperatorLowerEqual,      "<=",   -302,                                       prec::Comparative,    LeftToRight)
__bin_operator_def(OperatorGreaterEqual,    ">=",   -303,                                       prec::Comparative,    LeftToRight)
__bin_operator_def(OperatorEqual,           "==",   -304,                                       prec::Comparative,    LeftToRight)
__bin_operator_def(OperatorNotEqual,        "!=",   -305,                                       prec::Comparative,    LeftToRight)
__bin_operator_def(OperatorLogicalAnd,      "&&",   -306,                                       prec::LogicalAnd,     LeftToRight)
__bin_operator_def(OperatorLogicalOr,       "||",   -307,                                       prec::LogicalOr,      LeftToRight)
__bin_operator_def(OperatorAssign,          "=",    '=',                                        prec::Assignment,     RightToLeft)
__bin_operator_def(OperatorMultiplyEqual,   "*=",   OP_ATTACH_ASSIGNMENT(OperatorMultiply),     prec::Assignment,     RightToLeft)
__bin_operator_def(OperatorDivideEqual,     "/=",   OP_ATTACH_ASSIGNMENT(OperatorDivide),       prec::Assignment,     RightToLeft)
__bin_operator_def(OperatorRemainderEqual,  "%=",   OP_ATTACH_ASSIGNMENT(OperatorRemainder),    prec::Assignment,     RightToLeft)
__bin_operator_def(OperatorPlusEqual,       "+=",   OP_ATTACH_ASSIGNMENT(OperatorPlus),         prec::Assignment,     RightToLeft)
__bin_operator_def(OperatorMinusEqual,      "-=",   OP_ATTACH_ASSIGNMENT(OperatorMinus),        prec::Assignment,     RightToLeft)


__un_operator_def(OperatorExclMark,         "!",    '!')
__un_operator_def(OperatorTilde,            "~",    '~')













