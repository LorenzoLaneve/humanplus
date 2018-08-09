// => hpc/analyzers/syntax/operators.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_operators
#define __human_plus_compiler_operators

#define OP_COMPOUND_ASSIGNMENT -800

/*!
 \brief Returns whether the given token type is a compound assignment.
 */
#define OP_IS_COMPOUND_ASSIGNMENT(x) (x <= -700)

/*!
 \brief Returns the compound assignment for the given operator.
 \note If the operator does not support compound assignment, this macro will return a meaningless token type.
 */
#define OP_ATTACH_ASSIGNMENT(x) (x + OP_COMPOUND_ASSIGNMENT)
/*!
 \brief Returns the operator extracted from the given compound assignment.
 \note If the given operator is not a compound assignment, this macro will return a meaningless token type.
 */
#define OP_DETACH_ASSIGNMENT(x) (x - OP_COMPOUND_ASSIGNMENT)

namespace hpc {
    namespace syntax {
        /*!
         \brief Type for token types specified in this header. This is specified here only to avoid including tokens.h
         */
        typedef long OperatorToken;
        
        /*!
         \brief Type to represent and order precedences.
         */
        typedef unsigned int OperatorPrecedence;
        
        namespace prec {
            /*!
             \brief Predefined values for operators precedence.
             */
            typedef enum : unsigned {
                Exponentiative      = 300,
                Multiplicative      = 150,
                Additive            = 130,
                Comparative         = 110,
                LogicalAnd          = 90,
                LogicalOr           = 80,
                Assignment          = 50
            } PredefPrecedence;
        }
        
        /*!
         \brief Values indicating the associativity of a binary operator.
         */
        typedef enum {
            AssocLeftToRight,
            AssocRightToLeft
        } OperatorAssociativity;

        /*!
         \brief Returns the precedence value of the given binary operator token.
         */
        OperatorPrecedence getOperatorPrecedence(OperatorToken op);
        /*!
         \brief Returns the associativity value of the given binary operator token.
         */
        OperatorAssociativity getOperatorAssociativity(OperatorToken op);
        
        /*!
         \brief Returns whether the given token is a binary operator.
         */
        bool isBinaryOperator(OperatorToken op);
        /*!
         \brief Returns whether the given token is an unary operator.
         */
        bool isUnaryOperator(OperatorToken op);
        /*!
         \brief Returns whether the given token is an operator, either binary or unary.
         */
        inline bool isOperator(OperatorToken op) {
            return isBinaryOperator(op) || isUnaryOperator(op);
        }
    }
}

#endif
