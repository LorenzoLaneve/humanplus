//
//  binary.h
//  HumanPlus
//
//  Created by Lore on 11/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_ast_binary
#define __human_plus_compiler_ast_binary

#include <hpc/ast/exprs/expression.h>
#include <hpc/ast/types/base.h>
#include <hpc/analyzers/lexer/lexer.h>

#include <llvm/IR/Value.h>

namespace hpc {
    namespace ast {
        
        /*!
         \brief Abstract base class for all the binary expressions.
         */
        class BinaryExpr : public Expr {
        protected:
            /*!
             \brief The left-hand side expression.
             */
            Expr *lhs;
            /*!
             \brief The arithmetic operator.
             */
            lexer::token_ty oper;
            /*!
             \brief The right-hand side expression.
             */
            Expr *rhs;
            /*!
             \brief The "return type" of the binary expression.
             \note This \c ast::Type object will be \c nullptr until \c validate() is called.
             */
            Type *negotiatedType = nullptr;

        public:
            /*!
             \brief Initializes the object with the operands and the operator.
             */
            BinaryExpr(Expr *lhs, lexer::token_ty oper, Expr *rhs);
            virtual ~BinaryExpr() {  }
            
            inline Expr *getLHS() const { return lhs; }
            inline Expr *getRHS() const { return rhs; }
            
            virtual Type *evalType() = 0;
            
            inline lexer::token_ty getOperator() const { return oper; }
            
            inline void setExpressionType(ast::Type *newType) { negotiatedType = newType; }
            
            /*!
             \brief Replaces the left-hand side expression with its cast to the given type.
             */
            void castLHSToType(ast::Type *destination);
            /*!
             \brief Replaces the right-hand side expression with its cast to the given type.
             */
            void castRHSToType(ast::Type *destination);
            /*!
             \brief Replaces the compound assignment expression (if any) with an assignment expression with the operation as right-hand side.
             \note This method is an easy way to translate the compound assignment operations, but it prevents the code generator to see it as it has been specified in the source code and to apply any further optimizations.
             */
            void expandCompoundAssignment();
            
            
            llvm_rtti_impl_superclass(BinaryExpr);
            
            /*!
             \brief Returns a new \c ast::BinaryExpr object describing a binary expression between the two given expressions, with \c oper as binary operator.
             \note Since \c ast::BinaryExpr is abstract, you should use this method to create any binary expression. This method will use the right subclass depending on the binary operator.
             */
            static Expr *create(Expr *lhs, lexer::token_ty oper, Expr *rhs);
        };
        
        /*!
         \brief An object describing a binary expression for an arithmetic operation, such as addition, subtraction, multiplication, division and modulo.
         */
        class ArithmeticExpr : public BinaryExpr {
        public:
            ArithmeticExpr(Expr *lhs, lexer::token_ty oper, Expr *rhs) : BinaryExpr(lhs, oper, rhs) {  }

            virtual Type *evalType();
            
            
            llvm_rtti_impl(ArithmeticExpr);
        };
        
        /*!
         \brief An object describing a binary expression for a comparation operation, such as lower, greater, lower equal, greater equal, equal and not equal.
         */
        class ComparisonExpr : public BinaryExpr {
        public:
            ComparisonExpr(Expr *lhs, lexer::token_ty oper, Expr *rhs) : BinaryExpr(lhs, oper, rhs) {  }

            virtual Type *evalType();
            
            
            llvm_rtti_impl(ComparisonExpr);
        };
        
        /*!
         \brief An object describing a binary expression for an assignment operation.
         */
        class AssignmentExpr : public BinaryExpr {
        public:
            AssignmentExpr(Expr *lhs, lexer::token_ty oper, Expr *rhs) : BinaryExpr(lhs, oper, rhs) {  }
            
            virtual Type *evalType();
            
            virtual bool supportsLeftHandAssignment() const { return true; } // FIXME is it really true?

            
            llvm_rtti_impl(AssignmentExpr);
        };
        
        /*!
         \brief An object describing a binary expression for a bitwise operation.
         */
        class BitwiseExpr : public BinaryExpr {
        public:
            BitwiseExpr(Expr *lhs, lexer::token_ty oper, Expr *rhs) : BinaryExpr(lhs, oper, rhs) {  }
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(BitwiseExpr);
        };
        
    }
}

#endif

