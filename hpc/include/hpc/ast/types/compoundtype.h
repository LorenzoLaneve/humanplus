// => hpc/ast/types/compoundtype.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_type_compound
#define __human_plus_compiler_ast_type_compound

#include <hpc/ast/types/base.h>
#include <hpc/ast/decls/class.h>

#include <llvm/IR/Type.h>

#include <vector>

namespace hpc {
    namespace ast {

        class FieldDecl;
        
        /*!
         \brief Class for type instances of class-defined types.
         */
        class ClassType : public Type {
            friend class ClassDecl;
            
            /*!
             \brief The \c ast::ClassDecl object describing the Human Plus class that generated this type.
             */
            ClassDecl *declaration;
            
            
            ClassType(ClassDecl *declaration) : declaration(declaration) {  }
            
        public:
            
            inline const std::vector<ast::FieldDecl *> &getFields() const { return declaration->getFields(); }
            
            inline ClassDecl *getDeclarator() const { return declaration; }

            inline Type *getCanonicalType() { return this; }
            
            inline bool isCanonicalType() const { return true; }
            
            inline TypeFormat getFormat() const { return TypeFormatCompound; }
            
            bool canCastTo(Type *type, bool explicitly = false);
            
            bool canAssignTo(Type *type);
            
            FieldDecl *getMember(std::string memberID);
            
            
            std::string str(bool quoted = true);
            

            llvm_rtti_impl(ClassType);
        };
        
    }
}

#endif
