// => hpc/ast/stmt/whileuntil.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_iteration_whileuntil
#define __human_plus_compiler_ast_iteration_whileuntil

#include <hpc/ast/unit.h>
#include <hpc/ast/stmt/statement.h>
#include <hpc/ast/exprs/castings.h>

#include <llvm/IR/Value.h>

namespace hpc {
    namespace ast {
        
        /*!
         \brief Base abstract class for all the simple iteration statements such as \c while, \c until, \c do-while and \c do-until.
         */
        class SimpleIterStmt : public Stmt {
        public:
            typedef enum {
                IterationTypeWhile,
                IterationTypeUntil
            } IterationType;
            
        protected:
            /*!
             \brief The condition for the iteration.
             */
            EvalExpr *condition;
            /*!
             \brief The block that will be executed by the iteration.
             */
            Stmt *block;
            
        public:
            /*!
             \brief Initializes the iteration with the iteration condition and the enclosed block.
             */
            SimpleIterStmt(Expr *condition, Stmt *block);
            virtual ~SimpleIterStmt() {  }
            
            inline Expr *getCondition() const { return condition; }
            
            inline Stmt *getBlock() const { return block; }
            
            virtual bool returns() const;
            virtual int containedReturns() const;


            llvm_rtti_impl_superclass(SimpleIterStmt);
        };
        
        /*!
         \brief An object describing a pre-conditioned \c while statement.
         */
        class PreWhileStmt : public SimpleIterStmt {
        public:
            PreWhileStmt(Expr *condition, Stmt *block) : SimpleIterStmt(condition, block) {  }
            
            virtual break_target getBreakRole() const { return BreakWhile; };
            virtual continue_target getContinueRole() const { return ContinueWhile; };
            
            
            llvm_rtti_impl(PreWhileStmt);
        };
        
        /*!
         \brief An object describing a pre-conditioned \c until statement.
         */
        class PreUntilStmt : public SimpleIterStmt {
        public:
            PreUntilStmt(Expr *condition, Stmt *block) : SimpleIterStmt(condition, block) {  }
            
            virtual break_target getBreakRole() const { return BreakUntil; };
            virtual continue_target getContinueRole() const { return ContinueUntil; };
            
            
            llvm_rtti_impl(PreUntilStmt);
        };
        
        /*!
         \brief An object describing a post-conditioned \c while statement.
         */
        class PostWhileStmt : public SimpleIterStmt {
        public:
            PostWhileStmt(Expr *condition, Stmt *block) : SimpleIterStmt(condition, block) {  }
            
            virtual bool returns() const;
            
            virtual break_target getBreakRole() const { return BreakDoWhile; };
            virtual continue_target getContinueRole() const { return ContinueDoWhile; };
            
            
            llvm_rtti_impl(PostWhileStmt);
        };
        
        /*!
         \brief An object describing a post-conditioned \c until statement.
         */
        class PostUntilStmt : public SimpleIterStmt {
        public:
            PostUntilStmt(Expr *condition, Stmt *block) : SimpleIterStmt(condition, block) {  }
            
            virtual bool returns() const;
            
            virtual break_target getBreakRole() const { return BreakDoUntil; };
            virtual continue_target getContinueRole() const { return ContinueDoUntil; };
            
            
            llvm_rtti_impl(PostUntilStmt);
        };
        
    }
}

#endif
