// => hpc/ast/exprs/castings.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_expression
#define __human_plus_compiler_ast_expression

#include <hpc/ast/exprs/expression.h>
#include <hpc/ast/types/base.h>
#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/analyzers/syntax/operators.h>

#include <llvm/IR/Value.h>

namespace hpc {
    namespace ast {
        
        /*!
         \brief An object describing a trasformation of the values implicitly added by the compiler.
         */
        class ImplicitCastExpr : public Expr {
            /*!
             \brief The value which will be casted.
             */
            Expr *val;
            /*!
             \brief The destination type of the casting.
             */
            Type *destination;
            
        public:
            /*!
             \brief Initializes the implicit cast with the value to be casted and the type the value will be casted to.
             */
            ImplicitCastExpr(Expr *val, Type *destination) : val(val), destination(destination) {  }
            virtual ~ImplicitCastExpr() {  }
            
            /*!
             \brief Returns the expression to be casted.
             */
            inline Expr *getExpression() const { return val; }
            /*!
             \brief Returns the type the value will be casted to.
             */
            inline Type *getDestination() const { return destination; }
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(ImplicitCastExpr);
        };
        
        /*!
         \brief An object describing an evaluation, a conversion of a value to a boolean.
         */
        class EvalExpr : public Expr { // FIXME This should be integrated in ImplicitCastExpr
            /*!
             \brief The value to be evaluated.
             */
            Expr *conditionVal;
            
        public:
            /*!
             \brief Initializes the evaluator with a value to convert to boolean.
             */
            EvalExpr(Expr *conditionVal) : conditionVal(conditionVal) {  }
            virtual ~EvalExpr() {  }
            
            /*!
             \brief Returns the expression to be casted.
             */
            inline Expr *getExpression() const { return conditionVal; }
            
            virtual Type *evalType();
            
            
            llvm_rtti_impl(EvalExpr);
        };
        
    }
}

#endif
