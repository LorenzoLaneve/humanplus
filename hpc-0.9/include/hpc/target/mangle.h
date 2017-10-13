//
//  mangle.h
//  HumanPlus
//
//  Created by Lore on 14/05/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_target_mangle
#define __human_plus_compiler_target_mangle

#include <string>

namespace hpc {
    
    namespace ast {
        class NameSpaceDecl;
        class FunctionDecl;
        class GlobalVar;
        class Type;
        class ClassDecl;
    }
    
    namespace target {
        
        /*!
         \brief Class which defines an abstraction layer for ABI mangle specs used by the Human Plus Compiler.
         */
        class MangleSpec {
            
        public:
            virtual ~MangleSpec() {  }

            /*!
             \brief Returns a mangled name of the \c nameSpace object. The mangled name is generated conforming to the ABI mangle specifications.
             \param nameSpace An \c ast::NameSpaceDecl object <u>contained</u> in the AST representing the nameSpace entity we want to mangle.
             */
            virtual std::string mangleNameSpace(ast::NameSpaceDecl *nameSpace) = 0;
            /*!
             \brief Returns a mangled name of the \c function object. The mangled name is generated conforming to the ABI mangle specifications.
             \param function An \c ast::FunctionDecl object <u>contained</u> in the AST representing the function entity we want to mangle.
             \note \c mangleNameSpace() will be used to mangle any enclosing nameSpace, and \c mangleType() will be used to mangle types of any taking parameter.
             */
            virtual std::string mangleFunction(ast::FunctionDecl *function) = 0;
            /*!
             \brief Returns a mangled name of the \c variable object. The mangled name is generated conforming to the ABI mangle specifications.
             \param variable An \c ast::GlobalVar object <u>contained</u> in the AST representing the function entity we want to mangle.
             \note \c mangleNameSpace() will be used to mangle any enclosing nameSpace.
             */
            virtual std::string mangleGlobalVariable(ast::GlobalVar *variable) = 0;
            /*!
             \brief Returns a mangled name of the \c type object. The mangled name is generated conforming to the ABI mangle specifications.
             \param type An \c ast::Type object describing the type we want to mangle.
             \note \c mangleNameSpace() will be used to mangle any enclosing nameSpace.
             */
            virtual std::string mangleType(ast::Type *type) = 0;
            /*!
             \brief Returns a mangled name of the \c class object. The mangled name is generated conforming to the ABI mangle specifications.
             \param classDecl An \c ast::ClassDecl object <u>contained</u> in the AST representing the class entity we want to mangle.
             */
            virtual std::string mangleClass(ast::ClassDecl *classDecl) = 0;
            
            
            inline std::string mangleNameSpace(ast::NameSpaceDecl &nameSpace) {
                return mangleNameSpace(&nameSpace);
            }
            
            inline std::string mangleFunction(ast::FunctionDecl &function) {
                return mangleFunction(&function);
            }
            
            inline std::string mangleGlobalVariable(ast::GlobalVar &variable) {
                return mangleGlobalVariable(&variable);
            }
            
            inline std::string mangleType(ast::Type &type) {
                return mangleType(&type);
            }
            
            inline std::string mangleClass(ast::ClassDecl &classDecl) {
                return mangleClass(&classDecl);
            }
            
        };
        
        class NeuronMangle : public MangleSpec {
            
        public:
            std::string mangleNameSpace(ast::NameSpaceDecl *nameSpace);
            
            std::string mangleFunction(ast::FunctionDecl *function);
            
            std::string mangleGlobalVariable(ast::GlobalVar *variable);
            
            std::string mangleType(ast::Type *type);
            
            std::string mangleClass(ast::ClassDecl *classDecl);
            
        };
        
    }
}

#endif
