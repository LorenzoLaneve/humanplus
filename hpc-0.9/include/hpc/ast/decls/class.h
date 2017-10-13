//
//  ClassDecl.h
//  Human Plus
//
//  Created by Lore on 09/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#ifndef __human_plus_compiler_ast_class
#define __human_plus_compiler_ast_class

#include <hpc/ast/decls/declaration.h>
#include <hpc/ast/decls/namespace.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/unit.h>
#include <hpc/ast/types/base.h>

#include <vector>
#include <string>

namespace hpc {
    namespace ast {
        class FieldDecl;
        class ClassType;
        
        class ClassDecl : public NameSpaceDecl {
            friend class ClassType;
            
            /*!
             \brief The class superclass.
             */
            std::string base; // FIXME Symbols should be used.
            
            /*!
             \brief A \c std::vector containing all the Human Plus protocols this object conforms to.
             */
            std::vector<std::string> protocols; // FIXME Symbols should be used.
            

            std::vector<Decl *> members;
            
            std::vector<FieldDecl *> fieldv;
            
            std::map<std::string, FieldDecl *> fields;
            
            ClassType *classType;
            
        public:
            ClassDecl(std::string name, std::string base = "", std::vector<std::string> protocols = {});
            virtual ~ClassDecl() {  }
            
            inline const std::vector<Decl *> &getMembers() const { return members; }
            
            inline const std::vector<FieldDecl *> &getFields() const { return fieldv; }
            
            inline ClassType *getType() const { return classType; }

            FieldDecl *getFieldDecl(std::string memberid);
            
            void addField(FieldDecl *field);
            

            llvm_rtti_impl(ClassDecl);
        };
        
        /*!
         \brief An object describing a member variable contained in a class.
         */
        class FieldDecl : public GlobalVar { // FIXME maybe this shouldn't inherit GlobalVar...
        public:
            typedef int index_t;
            
        private:
            /*!
             \brief The \c ast::ClassDecl object describing the Human Plus class containing this field.
             */
            ClassDecl *containerClass = nullptr;
            
            /*!
             \brief The index of the field in the class declaration, starting from 0.
             */
            index_t index = -1;
            
        public:
            
            /*!
             \brief Initializes the class field with an identifier, a type, and an optional initial value.
             \note If \c type is \c nullptr, the type will be inferred from the initial value.
             */
            FieldDecl(std::string name, Type *type, ClassDecl *container, Expr *initval = nullptr);
            virtual ~FieldDecl() {  }

            ClassDecl *getContainerClass() const { return containerClass; }
            
            ClassType *getEnclosingType();
            
            index_t getFieldIndex() const { return index; }
            
            void setFieldIndex(index_t index);
            

            llvm_rtti_impl(FieldDecl);
        };
        
    }
}

#endif
