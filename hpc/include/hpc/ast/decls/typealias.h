// => hpc/ast/decls/typealias.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_typealias
#define __human_plus_compiler_ast_typealias

#include <hpc/ast/decls/declaration.h>
#include <hpc/ast/decls/namespace.h>
#include <hpc/ast/unit.h>
#include <hpc/ast/types/base.h>

#include <string>
#include <vector>

namespace hpc {
    namespace ast {
        
        class TypeAliasRef;
        
        /*!
         \brief An object describing a type alias. A type alias is an identifier which can be associated to a type, to create a more meaningful name in a specific context.
         */
        class TypeAliasDecl : public GlobalDecl {
            
            /*!
             \brief The aliased type. It may need to be validated. The type alias declaration will try to resolve it when \c validate() is called.
             */
            AliasedType *aliasedType;

        public:
            /*!
             \brief Initializes the type alias with an identifier for the alias, the aliased type, and the namespace that will contain this object.
             */
            TypeAliasDecl(std::string aliasName, Type *original, NameSpaceDecl *container)
                : GlobalDecl(aliasName, container), aliasedType(new AliasedType(this, original)) {  }
            
            virtual ~TypeAliasDecl() {  }
            
            /*!
             \brief Returns the type represented by this type alias.
             */
            inline AliasedType *getType() const { return aliasedType; }
            /*!
             \brief Returns the original type this type alias is masquerading.
             */
            inline Type *getOriginalType() const { return aliasedType->getOriginalType(); }
            

            llvm_rtti_impl(TypeAliasDecl);
        };
        
    }
}

#endif
