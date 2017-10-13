//
//  members.hpp
//  HumanPlus
//
//  Created by Lore on 01/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_ast_members
#define __human_plus_compiler_ast_members

#include <hpc/ast/exprs/expression.h>

#include <string>

#include <llvm/IR/Value.h>

namespace hpc {
    namespace ast {
        class FieldDecl;
        
        /*!
         \brief An object describing a field member access on an entity.
         */
        class FieldRef : public Expr {
            
            /*!
             \brief The entity with the member to access.
             */
            Expr *entity;
            
            /*!
             \brief The identifier of the field to be accessed.
             */
            std::string memberID;
            
            /*!
             \brief The declaration of the field in its type.
             \note This member is \c nullptr until the object is validated.
             */
            FieldDecl *declaration = nullptr;

        public:
            /*!
             \brief Initializes the member access operation with the entity and the field identifier.
             */
            FieldRef(Expr *entity, std::string memberID) : entity(entity), memberID(memberID) {  }
            virtual ~FieldRef() {  }
            
            inline Expr *getEntity() const { return entity; }
            
            inline std::string getMemberIdentifier() const { return memberID; }
            
            inline FieldDecl *getDeclaration() const { return declaration; }
            
            inline void setDeclaration(ast::FieldDecl *newDecl) { declaration = newDecl; }
            
            virtual Type *evalType();
            
            virtual bool supportsLeftHandAssignment() const { return true; }
            
            
            llvm_rtti_impl(FieldRef);
        };
        
    }
}

#endif
