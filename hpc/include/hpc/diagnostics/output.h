// => hpc/diagnostics/output.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_diagnostics_output
#define __human_plus_compiler_diagnostics_output

#include <hpc/utils/opts.h>

#include <llvm/Support/raw_ostream.h>

#include <vector>

namespace hpc {
    namespace diag {
        
        class Diagnostic;
        class DiagEngine;
        
        /*!
         \brief Abstract class indicating actions to be taken for reported diagnostics.
         */
        class DiagOutput {
            
        public:
            virtual ~DiagOutput() {  };
            
            /*!
             \brief Handles the diagnostic.
             */
            virtual void handleDiag(Diagnostic &diag) = 0;
            
        };
        
        /*!
         \brief Prints the incoming diagnostics in an output stream.
         */
        class DiagPrinter : public DiagOutput {
            
            llvm::raw_ostream &stream;
            
            opts::DiagnosticsOptions &opts;
            
        public:
            DiagPrinter(llvm::raw_ostream &stream, opts::DiagnosticsOptions &opts);
            
            void handleDiag(Diagnostic &diag);
            
        };
        
        /*!
         \brief Keeps the incoming diagnostics in a buffer and offers methods to report handled diagnostics to another diagnostics engine.
         */
        class DiagBufferer : public DiagOutput {
            
            std::vector<Diagnostic *> buffer;
            
        public:
            DiagBufferer() {  }

            ~DiagBufferer();
            
            
            void handleDiag(Diagnostic &diag);
            
            void passToEngine(DiagEngine &engine);
            
        };
        
    }
}

#endif
