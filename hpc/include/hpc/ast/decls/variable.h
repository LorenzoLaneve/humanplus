// => hpc/ast/decls/variable.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_var
#define __human_plus_compiler_ast_var

#include <hpc/ast/decls/declaration.h>
#include <hpc/ast/types/base.h>
#include <hpc/ast/exprs/expression.h>

#include <llvm/IR/Value.h>

#include <string>

namespace hpc {
    namespace ast {
        class NameSpaceDecl;
        
        /*!
         \brief Abstract base class for all the variable declarations.
         */
        class Var : public Decl {
        protected:
            /*!
             \brief The variable identifier.
             */
            std::string name;
            /*!
             \brief The variable type.
             */
            Type *type;
            
        public:
            /*!
             \brief Initializes the variable with an identifier and a type.
             */
            Var(std::string name, Type *type) : name(name), type(type) {  }
            virtual ~Var() {  }

            /*!
             \brief Returns the identifier for this variable.
             */
            inline std::string getName() const { return name; }
            /*!
             \brief Returns the variable type.
             */
            inline Type *getType() const { return type; }
            
            /*!
             \brief Returns an \c ast::Expr object describing the initial value for this variable, or \c nullptr if no initial value is specified.
             */
            virtual Expr *getInitialValue() const { return nullptr; }
            
            
            inline void setType(ast::Type *newType) { type = newType; }
            

            llvm_rtti_impl_superclass(Var);
        };
        
        /*!
         \brief An object describing a variable contained in a global scope.
         */
        class GlobalVar : public Var {
        protected:
            /*!
             \brief The expression given to the variable at initialization time.
             */
            Expr *initval;
            /*!
             \brief An \c ast::NameSpaceDecl object describing the Human Plus namespace containing this global variable.
             */
            NameSpaceDecl *container = nullptr;

        public:
            /*!
             \brief Initializes the global variable with an identifier, a type, and an optional initial value.
             \note If \c type is \c nullptr, the type will be inferred from the initial value.
             */
            GlobalVar(std::string name, Type *type, NameSpaceDecl *container, Expr *initval = nullptr)
            : Var(name, type), initval(initval), container(container) {  }
            
            virtual ~GlobalVar() {  }
            
            virtual ast::Expr *getInitialValue() const { return initval; }
            
            void setInitialValue(Expr *newVal) { initval = newVal; }
            
            NameSpaceDecl *getContainer() const { return container; }
            
            
            llvm_rtti_impl_superclass(GlobalVar);
        };
        
        /*!
         \brief An object describing a variable contained in a local scope.
         */
        class LocalVar : public Var {
            /*!
             \brief The expression given to the variable at initialization time.
             */
            Expr *initval;
            
        public:
            /*!
             \brief Initializes the local variable with an identifier, a type, and an optional initial value.
             \note If \c type is \c nullptr, the type will be inferred from the initial value.
             */
            LocalVar(std::string name, Type *type, Expr *initval = nullptr) : Var(name, type), initval(initval) {  }
            virtual ~LocalVar() {  }
            
            virtual ast::Expr *getInitialValue() const { return initval; }
            
            void setInitialValue(Expr *newVal) { initval = newVal; }
            

            llvm_rtti_impl(LocalVar);
        };
        
        /*!
         \brief An object describing a function parameter.
         */
        class ParamVar : public Var {
            /*!
             \brief The expression given to the variable as default value.
             */
            Expr *defval;
            
        public:
            /*!
             \brief Initializes the parameter variable with an identifier, a type, and an optional default value.
             \note Always check whether a variable with a default value is not declared in a function before a required variable.
             */
            ParamVar(std::string name, Type *type, Expr *defval = nullptr) : Var(name, type), defval(defval) {  }
            virtual ~ParamVar() {  }

            /*!
             \brief Returns an \c ast::Expr object describing the default value for this parameter variable, or \c nullptr if the parameter variable is required.
             */
            inline Expr *getDefaultValue() const { return defval; }
            
            void setDefaultValue(Expr *newVal) { defval = newVal; }
            

            llvm_rtti_impl(ParamVar);
        };
        
    }
}

#endif
