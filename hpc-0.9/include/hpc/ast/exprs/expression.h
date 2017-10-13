//
//  valueAST.h
//  Human Plus
//
//  Created by Lore on 12/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#ifndef __human_plus_compiler_ast_value
#define __human_plus_compiler_ast_value

#include <hpc/ast/symbols.h>
#include <hpc/ast/types/base.h>
#include <hpc/ast/stmt/statement.h>

#include <string>

namespace hpc {
    namespace ast {
        
        class FunctionDecl;
        
        /*!
         \brief Abstract base class for any assignable expression entity.
         */
        class Expr : public Stmt {
            
        public:
            virtual ~Expr() {  }
            
            /*!
             \brief A \c source::TokenRef object pointing to the whole value.
             \code 
             a + foo() - bar(x, y)
             ^~~~~~~~~~~~~~~~~~~~~
             \endcode
             */
            source::TokenRef *completeRef();

            /*!
             \brief Returns the type of this expression.
             */
            virtual Type *evalType() = 0;
            
            /*!
             \brief \c true if this value is a valid assignment left-hand side, \c false otherwise.
             */
            virtual bool supportsLeftHandAssignment() const { return false; }
            /*!
             \brief \c true if this value can be qualified as null pointer, \c false otherwise.
             */
            virtual bool isNullPointer() const { return false; }
            
            
            llvm_rtti_impl_superclass(Expr);
        };

    }
}

#endif







