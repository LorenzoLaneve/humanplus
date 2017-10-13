// => hpc/ast/decls/declaration.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_declaration
#define __human_plus_compiler_ast_declaration

#include <hpc/ast/component.h>
#include <hpc/ast/symbols.h>

#include <llvm/IR/Module.h>

#include <string>

namespace hpc {
    namespace ast {
        /*!
         \brief Base class for all the classes which compose AST objects describing any entity declarations.
         */
        class Decl : public Component {
            
        public:
            virtual ~Decl() {  }

            
            llvm_rtti_impl_superclass(Decl);
        };
        
        /*!
         \brief Base class for all the declarations which have a name.
         */
        class NamedDecl : public Decl {
            /*!
             \brief The declaration name.
             */
            std::string name;
            
        protected:
            NamedDecl(std::string name) : name(name) {  }
            
        public:
            virtual ~NamedDecl() {  }
            
            inline std::string getName() const { return name; }
            
            
            llvm_rtti_impl_superclass(NamedDecl);
        };
        
        /*!
         \brief Base class for all the declarations with a global name.
         */
        class GlobalDecl : public NamedDecl {
            /*!
             \brief The innermost namespace containing this declaration.
             */
            ast::NameSpaceDecl *containerNS = nullptr;
            
        public:
            GlobalDecl(std::string name, ast::NameSpaceDecl *containerNS = nullptr) : NamedDecl(name), containerNS(containerNS) {  }
            
            /*!
             \brief Returns the innermost namespace containing this declaration.
             */
            inline ast::NameSpaceDecl *getContainer() const { return containerNS; }
            
            /*!
             \brief Sets the container namespace to the given namespace.
             */
            inline void setContainer(ast::NameSpaceDecl *newContainer) { containerNS = newContainer; }
            
            /*!
             \brief Returns a \c ast::Symbol object containing the complete path to the declaration, starting from the root namespace.
             */
            ast::Symbol getSymbolPath() const;
            
            /*!
             \brief Returns a string containing a source-like representation of the complete symbol path to the declaration, starting from the root namespace.
             */
            inline std::string getStringPath() const { return getSymbolPath().str(); }
            
            
            llvm_rtti_impl_superclass(GlobalDecl);
        };
        
    }
}

#endif
