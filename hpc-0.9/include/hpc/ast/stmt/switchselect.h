//
//  switchselect.h
//  HumanPlus
//
//  Created by Lore on 26/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_ast_selection_switchselect
#define __human_plus_compiler_ast_selection_switchselect

#include <hpc/ast/stmt/statement.h>

#include <vector>

#include <llvm/IR/Value.h>

namespace hpc {
    namespace ast {
        
        /*!
         \brief An object describing a compound selection statement.
         */
        class SwitchStmt : public Stmt {
            /*!
             \brief The value to select a block of the selection with.
             */
            Expr *selectionVal;
            /*!
             \brief The block containing the cases to be selected.
             */
            Stmt *block;
            
        public:
            /*!
             \brief Initializes the compound selection statement with the selection value and the block.
             */
            SwitchStmt(Expr *selectionVal, Stmt *block);
            virtual ~SwitchStmt() {  }
            
            inline Expr *getSelectionValue() const { return selectionVal; }
            
            inline Stmt *getBlock() const { return block; }
            
            
            //llvm_rtti_impl(SwitchStmt);
        };
        
    }
}

#endif
