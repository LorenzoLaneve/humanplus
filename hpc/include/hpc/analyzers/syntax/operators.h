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

#define OP_COMPOUND_ASSIGNMENT -500
#define OP_IS_COMPOUND_ASSIGNMENT(x) (x < -400)

#define OP_ATTACH_ASSIGNMENT(x) (x + OP_COMPOUND_ASSIGNMENT)
#define OP_DETACH_ASSIGNMENT(x) (x - OP_COMPOUND_ASSIGNMENT)

namespace hpc {
    namespace lexer {
        typedef int token_ty; // FIXME
    }
    
    namespace util {
        
        /*!
         \brief Prints \c oper as operator symbol
         \param oper A \c Token value describing the operator.
         \warning If \c !syntax::isOperator(oper) the output is undefined.
         */
        void printOperator(lexer::token_ty oper);
    }
    
    namespace syntax {
        
        /*!
         \brief Returns a number indicating the precedence of the given operator. Operators with higher precedence will be executed first. 
         \code
         5 + 3 * 4 = 5 + (3 * 4)
         because precedence(*) > precedence(+)
         \endcode
         \note If \c !syntax::isOperator(token), 0 will be returned.
         */
        int getOperatorPrecedence(lexer::token_ty token);
    
        /*!
         \brief Returns whether the given token is an unary operator.
         \returns \c true if \c token is an unary operator recognized by the lexer, \c false otherwise.
         */
        bool isUnaryOperator(lexer::token_ty token);
        
        /*!
         \brief Returns whether the given token is a binary operator.
         \returns \c true if \c token is a binary operator recognized by the lexer, \c false otherwise.
         */
        bool isBinaryOperator(lexer::token_ty token);
        
        /*!
         \brief Returns whether the given token is an operator.
         \returns \c true if \c token is an operator recognized by the lexer, \c false otherwise.
         */
        bool isOperator(lexer::token_ty token);
        
    }
}

#endif
