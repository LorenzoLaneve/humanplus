//
//  objsynth.hpp
//  HumanPlus
//
//  Created by Lore on 19/03/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_backend_helper
#define __human_plus_compiler_backend_helper

#include <hpc/diagnostics/diagnostics.h>
#include <hpc/target/target.h>
#include <hpc/utils/files.h>
#include <hpc/utils/opts.h>

#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

#include <string>
#include <vector>

namespace hpc {
    namespace backend {
        
        /*!
         \brief Object that provides function for middle-end optimizations and back-end translations.
         */
        class BackendHelper {
        public:
            /*!
             \brief Values indicating the type of output of the backend.
             */
            typedef enum {
                EmitNothing         = 0,    ///< The backend will discard any output (used for testing purposes)
                EmitObjectFile      = 1,    ///< The backend will emit output as object code (.o/.obj)
                EmitAssemblyFile    = 2,    ///< The backend will emit output as textual assembly code (.s)
                EmitIRLLFile        = 3,    ///< The backend will emit output as textual LLVM IR file (.ll)
                EmitIRBCFile        = 4,    ///< The backend will emit output as binary LLVM IR file (.bc)
            } EmitType;
            
        private:
            
            /*!
             \brief The diagnostics engine that should report any backend error.
             */
            diag::DiagEngine &diags;
            
            /*!
             \brief All the backend options.
             */
            opts::BackendOptions &backendOptions;
            /*!
             \brief All the target options.
             */
            opts::TargetOptions &targetOptions;
            
            /*!
             \brief The TargetInfo object containing information about the target we're compiling for.
             */
            target::TargetInfo *targetInfo;
            
            /*!
             \brief The \c PassManagerBuilder object the backend helper will use to create the pass managers for the incoming modules.
             */
            llvm::PassManagerBuilder managerBuilder;

            /*!
             \brief The per-function pass manager the middle-end will apply to the incoming modules.
             */
            llvm::legacy::FunctionPassManager *fpm;
            /*!
             \brief The per-module pass manager the middle-end will apply to the incoming modules.
             */
            llvm::legacy::PassManager *mpm;
            
            /*!
             \brief The module bound to the backend helper.
             */
            llvm::Module *boundmodule = nullptr;
            
        public:
            /*!
             \brief Initializes the helper with the type of output for the backend.
             */
            BackendHelper(diag::DiagEngine &diags, opts::BackendOptions &backendOptions, target::TargetInfo *targetInfo);
            
            /*!
             \brief Binds the given target info to the backend helper, which the backend will generate code for.
             */
            void bindTarget(target::TargetInfo *target);
            /*!
             \brief Binds the given module to the backend helper, and prepares the pass managers.
             */
            void bindModule(llvm::Module &module);
            
            /*!
             \brief Runs the per-module and per-function pass managers to the bound module.
             */
            void runPasses();
            
            /*!
             \brief Emits the bound module to the given output file. The output type depends on the file type.
             */
            bool emit(fsys::OutputFile *outfile);
            
            /*!
             \brief Returns the info about the target we're generating code for.
             */
            target::TargetInfo &getTargetInfo() const {
                return *targetInfo;
            }
            
        };
        
        /*!
         \brief Initializes the compiler backend, initializing the pass registry passes in order to prepare the compiler for the compilation process.
         */
        void init(diag::DiagEngine &diags, opts::BackendOptions &backendOptions, opts::TargetOptions &targetOptions);

    }
}

#endif
