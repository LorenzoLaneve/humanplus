//
//  BuiltinType.hpp
//  Human Plus
//
//  Created by Lore on 22/11/15.
//  Copyright © 2015 Snow Apps inc. All rights reserved.
//

#ifndef __human_plus_compiler_ast_type_builtin
#define __human_plus_compiler_ast_type_builtin

#include <hpc/ast/types/base.h>
//#include <hpc/ast/visitor.h>

#include <llvm/IR/Type.h>

#include <string>

namespace hpc {
    namespace ast {
        
        /*!
         \brief Class for type instances of built-in types defined by the language.
         */
        class BuiltinType : public Type {
            
        public:
            /*!
             \brief Enumeration containing all the possibly indexes for built-in types defined by the language.
             */
            typedef enum {
                
#define __builtintype(ID, SRCID, FORMAT, MAGNITUDE) ID,
#include <hpc/inc/builtintypes.inc>
#undef __builtintype
                
            } TypeID;
            
            /*!
             \brief Integer used to quickly verify if numeric types can be contained by other numeric types. E.g.: An integer type has a greater magnitude than a long integer type.
             \note Although type sizes are platform-independent as much as possible, these values are not the actual sizes that will be used in the IRs. They keep the order of sizes and are a quick implementation of a simple compatibility check between the built-in types. The magnitude of types that have nothing to do with numeric types will be ignored.
             */
            typedef uint64_t TypeMagnitude;
            
        private:
            TypeID typeID;
            
            BuiltinType(TypeID typeID) : typeID(typeID) {  }
            
        public:
            
            /*!
             \brief Returns the canonical type for the given built-in type ID.
             */
            static BuiltinType *get(TypeID typeID);
            
            /*!
             \brief Returns the type ID for the built-in type.
             */
            inline TypeID getBuiltinTypeID() const { return typeID; }
            
            ast::BuiltinType::TypeMagnitude getMagnitude() const;
            
            /*!
             \brief Returns the source-like identifier for the built-in type.
             */
            std::string getIdentifier() const;
            
            
            inline Type *getCanonicalType() { return this; }
            
            inline bool isCanonicalType() const { return true; }
            
            TypeFormat getFormat() const;
            
            bool canCastTo(Type *type, bool explicitly = false);

            bool canAssignTo(Type *type);
            
            std::string str(bool quoted = true);


            llvm_rtti_impl(BuiltinType);
        };
        
    }
}

#endif
