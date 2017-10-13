// => hpc/ast/exprs/unary.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_unary
#define __human_plus_compiler_ast_unary

#include <hpc/ast/exprs/expression.h>
#include <hpc/ast/types/base.h>
#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/analyzers/syntax/operators.h>

#include <llvm/IR/Value.h>

namespace hpc {
    
    namespace ast {
                
        /*!
         \brief Abstract base class for all the unary expressions.
         */
        class UnaryExpr : public Expr {
        protected:
            /*!
             \brief The value the unary operator will be applied to.
             */
            Expr *exp;
            
        public:
            /*!
             \brief Initializes the unary expression with an inner expression.
             */
            UnaryExpr(Expr *exp) : exp(exp) {  }
            virtual ~UnaryExpr() {  }
            
            inline Expr *getOperand() const { return exp; }
            
            /*!
             \brief Replaces the operand with its casting expression to boolean.
             */
            void castOperandToBoolean();
            
            virtual lexer::token_ty getOperator() const = 0;
            
            virtual Type *evalType() = 0;
            
            
            llvm_rtti_impl_superclass(UnaryExpr);
        };
        
        /*!
         \brief An object describing an unary expression with the minus sign as unary operator (arithmetic negation).
         */
        class ArithmeticNegationExpr : public UnaryExpr {
        public:
            ArithmeticNegationExpr(Expr *exp) : UnaryExpr(exp) {  }
            
            virtual lexer::token_ty getOperator() const { return lexer::TokenOperatorMinus; }

            virtual Type *evalType();
                        
            
            llvm_rtti_impl(ArithmeticNegationExpr);
        };
        
        /*!
         \brief An object describing an unary expression with the exclamation mark sign as unary operator (logical negation).
         */
        class LogicalNegationExpr : public UnaryExpr {
        public:
            LogicalNegationExpr(Expr *exp) : UnaryExpr(exp) {  }
            
            virtual lexer::token_ty getOperator() const { return lexer::TokenOperatorExclMark; }
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(LogicalNegationExpr);
        };
        
        /*!
         \brief An object describing an unary expression with the tilde sign as unary operator (bitwise negation).
         */
        class BitwiseNegationExpr : public UnaryExpr {
        public:
            BitwiseNegationExpr(Expr *exp) : UnaryExpr(exp) {  }
            
            virtual lexer::token_ty getOperator() const { return lexer::TokenOperatorTilde; }
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(BitwiseNegationExpr);
        };
        
    }
    
}

#endif
