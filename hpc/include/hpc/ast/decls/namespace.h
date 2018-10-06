// => hpc/ast/decls/namespace.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_nameSpace
#define __human_plus_compiler_ast_nameSpace

#include <hpc/ast/unit.h>
#include <hpc/ast/symbols.h>
#include <hpc/ast/types/base.h>
#include <hpc/ast/decls/declaration.h>

#include <string>
#include <vector>
#include <map>

namespace hpc {
    namespace validator {
        class SymbolPrevResolver;
    }
    
    namespace ast {
        
        class TypeAliasDecl;
        class FunctionDecl;
        class ClassDecl;
        class ProtocolDecl;
        class GlobalVar;
        
        typedef std::vector<ast::FunctionDecl *> OverloadList;
        
        /*!
         \brief An object describing an Human Plus \c namespace.
         */
        class NameSpaceDecl : public GlobalDecl {
            friend class CompilationUnit;
            friend class validator::SymbolPrevResolver;
            
            
            
            typedef std::map<std::string, NameSpaceDecl *> namespace_table;
            typedef std::map<std::string, OverloadList> func_table;
            typedef std::map<std::string, GlobalVar *> var_table;
            typedef std::map<std::string, Type *> type_table;
            
            /*!
             \brief Array of all the declarations contained in this namespace.
             */
            std::vector<Decl *> declarations;
            /*!
             \brief Array of all the type aliases declared in this namespace.
             */
            std::vector<TypeAliasDecl *> typeAliases;
            /*!
             \brief Array of all the classes declared in this namespace.
             */
            std::vector<ClassDecl *> classes;
            /*!
             \brief Array of all the protocols declared in this namespace.
             */
            std::vector<ProtocolDecl *> protocols;
            
            /*!
             \brief Map of all the namespaces contained in this namespace, where the indexes are the namespace identifiers.
             */
            namespace_table namespaces;
            /*!
             \brief Map of all the function lists contained in this namespace, where the indexes are the function identifiers.
             */
            func_table functions;
            /*!
             \brief Map of all the global variables contained in this namespace, where the indexes are the variable identifiers.
             */
            var_table globalVars;
            /*!
             \brief Map of all the types contained in this namespace, where the indexes are the type identifiers.
             */
            type_table types;
            
        public:
            /*!
             \brief Initializes the namespace with an identifier. Libraries without identifiers should only be used for AST root namespaces.
             \note Check if the global scope that will contain this object doesn't already have an object with this identifier.
             */
            NameSpaceDecl(std::string name = "", ast::NameSpaceDecl *containerNS = nullptr) : GlobalDecl(name, containerNS) {  }
            
            virtual ~NameSpaceDecl() {  }

            inline const std::vector<ast::Decl *> &getDeclarations() const { return declarations; }
            
            /*!
             \brief Adds the given namespace as inner namespace to this namespace.
             */
            void addInnerNameSpace(NameSpaceDecl *ns);
            /*!
             \brief Adds the given type alias declaration to this namespace.
             */
            void addTypeAlias(TypeAliasDecl *tpa);
            /*!
             \brief Adds the given global variable to this namespace.
             */
            void addGlobalVariable(GlobalVar *gvr);
            /*!
             \brief Adds the given function declaration to this namespace.
             */
            void addFunction(FunctionDecl *fnc);
            /*!
             \brief Adds the given class declaration to this namespace.
             */
            void addClass(ClassDecl *cls);
            /*!
             \brief Adds the given protocol declaration to this namespace.
             */
            void addProtocol(ProtocolDecl *ptc);
            /*!
             \brief Adds the given type to the type declarations contained in this namespace.
             */
            void addType(std::string identifier, Type *type);
            
            /*!
             \brief Returns an \c ast::NameSpaceDecl object describing the namespace matching the given \c ast::Symbol.
             */
            NameSpaceDecl *getInnerNameSpace(ast::Symbol sympath);
            /*!
             \brief Returns an \c ast::NameSpaceDecl object describing the namespace matching the given \c ast::SymbolIdentifier.
             */
            inline NameSpaceDecl *getInnerNameSpace(ast::SymbolIdentifier symbolID) {
                return getInnerNameSpace(Symbol(symbolID));
            }
            
            /*!
             \brief Returns an \c ast::GlobalVar object describing the global variable matching the given \c ast::Symbol.
             */
            GlobalVar *getGlobalVariable(ast::Symbol sympath);
            /*!
             \brief Returns an \c ast::GlobalVar object describing the global variable matching the given \c ast::SymbolIdentifier.
             */
            inline GlobalVar *getGlobalVariable(ast::SymbolIdentifier symbolID) {
                return getGlobalVariable(Symbol(symbolID));
            }
            
            /*!
             \brief Returns an \c OverloadList containing all the function of the namespace matching the given \c ast::Symbol.
             */
            const OverloadList *getOverloads(ast::Symbol sympath);
            /*!
             \brief Returns an \c OverloadList containing all the function of the namespace matching the given \c ast::SymbolIdentifier.
             */
            inline const OverloadList *getOverloads(ast::SymbolIdentifier symbolID) {
                return getOverloads(Symbol(symbolID));
            }
            
            /*!
             \brief Returns the type matching the given \c ast::Symbol.
             */
            Type *getType(ast::Symbol sympath);
            
            /*!
             \brief Returns the type matching the given \c ast::SymbolIdentifier.
             */
            inline Type *getType(ast::SymbolIdentifier symbolID) {
                return getType(Symbol(symbolID));
            }
            
            /*!
             \brief Returns whether the namespace contains a declaration matching the given symbol.
             */
            bool hasDeclaration(ast::Symbol sympath);
            

            llvm_rtti_impl(NameSpaceDecl);
        };
        
    }
}

#endif
