// => hpc/frontend/instance.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_instance
#define __human_plus_compiler_instance

#include <hpc/diagnostics/diagnostics.h>
#include <hpc/frontend/invocation.h>
#include <hpc/utils/files.h>

namespace hpc {
    
    class CompilerInvocation;
    
    /*!
     \brief Object indicating an instance of the Human Plus Compiler.
     */
    class CompilerInstance {
        friend class CompilerInvocation;
        
        /*!
         \brief The invocation of this compiler instance.
         */
        CompilerInvocation *invocation;
        /*!
         \brief The diagnostics engine used by the compiler to report diagnostics.
         */
        diag::DiagEngine *diags;
        /*!
         \brief The diagnostic printer.
         */
        diag::DiagPrinter *printer;
        
    public:
        CompilerInstance() {  }
        
        ~CompilerInstance();
        
        /*!
         \brief Returns the invocation of this compiler instance.
         */
        inline CompilerInvocation &getInvocation() const {
            assert("No invocation created for this compiler instance.");
            return *invocation;
        }
        /*!
         \brief Sets the invocation for this compiler instance.
         */
        inline void setInvocation(CompilerInvocation &invoke) {
            invocation = &invoke;
        }
        
        /*!
         \brief Creates a diagnostics engine which will be used by the compiler.
         */
        void createDiagnostics();
        /*!
         \brief Returns whether the compiler has a ready diagnostics engine.
         */
        inline bool hasDiagnostics() const {
            return diags != nullptr;
        }
        /*!
         \brief Returns the diagnostics engine used by the compiler.
        */
        inline diag::DiagEngine &getDiagnostics() const {
            assert(hasDiagnostics() && "The compiler instance doesn't have a diagnostics engine ready.");
            return *diags;
        }
        
        /*!
         \brief Returns whether the compiler instance can handle the given input file.
         */
        bool canHandleFile(fsys::File &file);
        
        inline opts::FrontendOptions &getFrontendOptions() const {
            return invocation->frontendOptions;
        }
        
        inline opts::DiagnosticsOptions &getDiagOptions() const {
            return invocation->diagOptions;
        }
        
        inline opts::TargetOptions &getTargetOptions() const {
            return invocation->targetOptions;
        }
        
        inline opts::BackendOptions &getBackendOptions() const {
            return invocation->backendOptions;
        }
        
        inline opts::LinkOptions &getLinkerOptions() const {
            return invocation->linkOptions;
        }
        
        /*!
         \brief Starts a frontend session with the bound invocation.
         */
        bool executeInvocation();
        
    };
}

#endif
