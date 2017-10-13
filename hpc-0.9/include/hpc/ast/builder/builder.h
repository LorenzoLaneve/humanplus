//
//  builder.h
//  HumanPlus
//
//  Created by Lore on 13/03/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_ast_builder
#define __human_plus_compiler_ast_builder

#include <hpc/utils/files.h>
#include <hpc/ast/ast.h>

#include <string>
#include <vector>

namespace hpc {
    
    namespace ast {
        
        class Builder {
            
            /*!
             \brief The tree being built.
             */
            ast::AbstractSyntaxTree *tree;
            /*
             \brief The unit the builder is currently adding declarations in.
             */
            ast::CompilationUnit *insertUnit;
            /*!
             \brief The insert point in the AST. All the top-level declaration built will have the value of this field at the moment of construction.
             */
            ast::NameSpaceDecl *insertNameSpace;
            
        public:
            /*!
             \brief Initializes the builder with the Abstract Syntax Tree (AST) to be built.
             */
            Builder(ast::AbstractSyntaxTree *tree);
            
            /*!
             \brief Returns the building tree.
             */
            inline ast::AbstractSyntaxTree *getTree() const {
                return tree;
            }
            
            /*!
             \brief Gets or creates an in-memory compilation unit for the given input file.
             */
            ast::CompilationUnit *getOrCreateUnit(fsys::InputFile *ifile);
            
            /*!
             \brief Sets the given unit to be used as insert point for incoming declarations.
             */
            void setInsertUnit(ast::CompilationUnit *unit) {
                insertUnit = unit;
            }
            
            void setInsertNameSpace(ast::NameSpaceDecl *nameSpace) {
                insertNameSpace = nameSpace;
            }
            
            void setInsertNameSpaceToParent() {
                insertNameSpace = insertNameSpace->getContainer();
            }
            
            /*!
             \brief Returns the current insert unit.
             */
            inline ast::CompilationUnit &getInsertUnit() const {
                assert(insertUnit && "No insert unit.");
                return *insertUnit;
            }
            
            /*!
             \brief Returns the current insert nameSpace.
             */
            inline ast::NameSpaceDecl &getInsertNameSpace() const {
                assert(insertNameSpace && "No insert namespace.");
                return *insertNameSpace;
            }
            
            /*!
             \brief Creates a namespace and adds it as inner namespace of the given namespace.
             \return The newly created namespace.
             */
            ast::NameSpaceDecl *getOrCreateNameSpace(std::string name);
            
            /*!
             \brief Creates a global variable declaration and adds it in the current insert unit.
             */
            ast::GlobalVar *createGlobalVariable(std::string varName, ast::Type *type, ast::Expr *initialValue);
            
            /*!
             \brief Creates a function declaration and adds it in the current insert unit.
             */
            ast::FunctionDecl *createFunctionDecl(std::string functionName, std::vector<ParamVar *> arguments, Type *returnType,
                                                  FunctionDecl::FunctionAttributes attrs = {});
            
            /*!
             \brief Creates a type alias declaration and adds it in the current insert unit.
             */
            ast::TypeAliasDecl *createTypeAliasDecl(std::string aliasName, ast::Type *originalType);
            
            /*!
             \brief Create a class declaration and adds it in the current insert unit.
             */
            ast::ClassDecl *createClassDecl(std::string className, ast::Symbol *superClass = nullptr, std::vector<ast::Symbol *> protocols = {});
            
        };
        
    }
    
}


#endif
