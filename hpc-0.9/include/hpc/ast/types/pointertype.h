//
//  PointerType.hpp
//  HumanPlus
//
//  Created by Lore on 13/12/15.
//  Copyright © 2015 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_ast_type_pointer
#define __human_plus_compiler_ast_type_pointer

#include <hpc/ast/types/base.h>

#include <llvm/IR/Type.h>

namespace hpc {
    namespace ast {
        
        /*!
         \brief Class for type instances of pointer types.
         */
        class PointerType : public Type {
            /*!
             \brief The type pointed by this type. May not be canonical. If this field is \c nullptr , the type is the null pointer type.
             */
            Type *pointedType;
            

            PointerType(Type *pointedType) : pointedType(pointedType) {  }
            
        public:
            /*!
             \brief Returns a pointer type to the given type.
             */
            static PointerType *get(Type *type);
            

            inline Type *getCanonicalType() {
                return pointedType ? get(pointedType->getCanonicalType()) : this;
            }
            
            inline bool isCanonicalType() const {
                return pointedType ? pointedType->isCanonicalType() : true;
            }
            
            inline TypeFormat getFormat() const { return TypeFormatPointer; }
            
            bool canCastTo(Type *type, bool explicitly = false);
            
            bool canAssignTo(Type *type);
            
            
            inline Type *getPointedType() const { return pointedType; }
            
            std::string str(bool quoted = true);
            

            llvm_rtti_impl(PointerType);
        };
        
    }
}

#endif
