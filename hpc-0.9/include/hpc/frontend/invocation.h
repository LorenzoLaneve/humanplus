//
//  invocation.h
//  HumanPlus
//
//  Created by Lore on 09/01/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_invocation
#define __human_plus_compiler_invocation

#include <hpc/utils/opts.h>
#include <hpc/diagnostics/diagnostics.h>

#include <string>
#include <vector>

namespace hpc {
    
    class CompilerInstance;
    
    /*!
     \brief Object indicating an invocation of the compiler.
     */
    class CompilerInvocation {
        friend class CompilerInstance;
        
        /*!
         \brief The compiler instance being invoked.
         */
        CompilerInstance &compiler;
        /*!
         \brief All the options about frontend actions.
         */
        opts::FrontendOptions frontendOptions;
        /*!
         \brief All the options about diagnostics reporting.
         */
        opts::DiagnosticsOptions diagOptions;
        /*!
         \brief All the options about target.
         */
        opts::TargetOptions targetOptions;
        /*!
         \brief All the options about backend.
         */
        opts::BackendOptions backendOptions;
        /*!
         \brief All the options about linker invocation.
         */
        opts::LinkOptions linkOptions;
        
        /*!
         \brief Initializes the compiler invocation with the compiler instance to be invoked.
         */
        CompilerInvocation(CompilerInstance &compiler) : compiler(compiler) {  }
        
    public:
        /*!
         \brief Creates an invocation for the given compiler with the given command line args and a diagnostics engine to report any error about the passed args.
         */
        static bool create(CompilerInstance &compiler, const char **argBegin, const char **argEnd, diag::DiagEngine &diags);
        
        inline CompilerInstance &getCompiler() const {
            return compiler;
        }
        
        
        inline opts::FrontendOptions &getFrontendOptions() {
            return frontendOptions;
        }
        
        inline opts::DiagnosticsOptions &getDiagOptions() {
            return diagOptions;
        }
        
        inline opts::TargetOptions &getTargetOptions() {
            return targetOptions;
        }
        
        inline opts::BackendOptions &getBackendOptions() {
            return backendOptions;
        }
        
        inline opts::LinkOptions &getLinkerOptions() {
            return linkOptions;
        }
        
    };
    
}

#endif
