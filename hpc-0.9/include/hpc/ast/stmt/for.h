//
//  for.hpp
//  HumanPlus
//
//  Created by Lore on 20/05/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_ast_iteration_for
#define __human_plus_compiler_ast_iteration_for

#include <hpc/ast/stmt/statement.h>
#include <hpc/ast/stmt/whileuntil.h>
#include <hpc/ast/exprs/expression.h>

#include <vector>

#include <llvm/IR/Value.h>

namespace hpc {
    namespace ast {
              
        /*!
         \brief An object describing a C-like \c for statement.
         */
        class ForStmt : public SimpleIterStmt {
            /*!
             \brief A \c std::vector containing the statements that will be executed before starting the iteration, specified in the leftmost clause of the statement.
             */
            std::vector<Stmt *> initstmt;
            /*!
             \brief A \c std::vector containing the statements that will be executed at the end of each iteration step, specified in the rightmost clause of the statement.
             */
            std::vector<Stmt *> endstmt;
            
        public:
            /*!
             \brief Initializes the C-like \c for statement with the three clauses.
             */
            ForStmt(std::vector<Stmt *> initstmt, Expr *condition, std::vector<Stmt *> endstmt, Stmt *block);
            virtual ~ForStmt() {  }
            
            inline const std::vector<Stmt *> &getInitStatements() const { return initstmt; }
            
            inline const std::vector<Stmt *> &getEndStatements() const { return endstmt; }
            
            virtual break_target getBreakRole() const { return BreakFor; };
            virtual continue_target getContinueRole() const { return ContinueFor; };
            
            
            llvm_rtti_impl(ForStmt);
        };
        
    }
}

#endif
