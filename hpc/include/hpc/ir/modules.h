// => hpc/ir/modules.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ir_modules
#define __human_plus_compiler_ir_modules

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include <string>
#include <vector>

namespace hpc {
    
    namespace ast {
        class Decl;
    }
    
    namespace modules {
        
        /*!
         \brief An object that handles the creation of an LLVM module.
         */
        class ModuleWrapper {
            /*!
             \brief The LLVM IR module this object is handling.
             */
            llvm::Module *module = nullptr;
            
            /*!
             \brief A \c std::vector containing all the AST top level declarations that will be built into the wrapped module.
             */
            std::vector<ast::Decl *> declarations;
            
        public:
            /*!
             \brief Initializes the module wrapper.
             \param modulename The name that the new module will have
             \param context The LLVM context to give to the new module
             */
            ModuleWrapper(std::string modulename, llvm::LLVMContext &context);
            virtual ~ModuleWrapper() {  }
            
            /*!
             \brief Returns the LLVM IR module wrapped by this object.
             */
            llvm::Module &getModule();
            
            /*!
             \brief Adds the first information to the wrapped LLVM IR module.
             */
            void initialize();
            
            /*!
             \brief Adds the last information to the wrapped LLVM IR module, completing it.
             */
            void finalize();
            
            /*!
             \brief Prints the wrapped module to the standard output.
             */
            void dump();
            
            /*!
             \brief Adds the AST declaration to the declarations that will be built in the wrapped module.
             */
            void addDeclaration(ast::Decl *declaration);
        };

    }
}

#endif
