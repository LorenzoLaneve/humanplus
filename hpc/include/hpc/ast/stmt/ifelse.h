// => hpc/ast/stmt/ifelse.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_selection_ifelse
#define __human_plus_compiler_ast_selection_ifelse

#include <hpc/ast/stmt/statement.h>
#include <hpc/ast/unit.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/ir/modules.h>

#include <llvm/IR/Value.h>

namespace hpc {
    namespace ast {
        
        class IfStmt : public Stmt {
            /*!
             \brief The expression as condition for the \c if statement.
             */
            EvalExpr *condition;
            /*!
             \brief The statement that will be executed if the condition is \c true.
             */
            Stmt *thenBlock;
            /*!
             \brief The statement that will be executed if the condition is \c false, or \c nullptr if there is no \c else clause.
             */
            Stmt *elseBlock;
            
        public:
            /*!
             \brief Initializes the \c if statement.
             \param condition A value that will be the condition of the selection statement.
             \param thenBlock A statement that will be executed if the statement is \c true (\c then block).
             \param elseBlock A statement that will be executed if the statement is \c false (\c else block).
             */
            IfStmt(Expr *condition, Stmt *thenBlock, Stmt *elseBlock = nullptr);
            virtual ~IfStmt() {  }
            
            inline Expr *getCondition() const { return condition; }
            
            inline Stmt *getThenBlock() const { return thenBlock; }
            inline Stmt *getElseBlock() const { return elseBlock; }
            
            inline bool hasElseBlock() const { return elseBlock != nullptr; }
            
            virtual bool returns() const;
            virtual int containedReturns() const;
            
            
            llvm_rtti_impl(IfStmt);
        };
        
    }
}

#endif
