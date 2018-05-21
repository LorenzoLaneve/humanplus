// => hpc/diagnostics/diagnostics.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_diagnostics
#define __human_plus_compiler_diagnostics

#include <hpc/diagnostics/output.h>
#include <hpc/analyzers/sources.h>
#include <hpc/utils/opts.h>

#include <llvm/Support/raw_ostream.h>

#include <string>

namespace hpc {
    
    namespace diag {
        
        class DiagEngine;
        class DiagnosticsReport;
        class DiagOutput;
        
        /*!
         \brief Levels of diagnostics.
         */
        typedef enum {
            Error,      ///< Error: diagnostic which prevents compilation.
            Warning,    ///< Warning: important diagnostics to report to the user.
            Note,       ///< Note: additional informations.
            Blank       ///< Blank type to print custom reports.
        } DiagLevel;
        
        /*!
         \brief Values indicating types of predefined diagnostics that can be shown to the user.
         */
        typedef enum {

#define __predef_diag(ID, desc) ID,
#include <hpc/inc/diags.inc>
            
#undef __predef_diag
            
        } DiagID;
        
        /*!
         \brief Object representing a diagnostic.
         */
        class Diagnostic {
            friend class DiagEngine;
            friend class DiagnosticsReport;
            
            /*!
             \brief The diagnostics engine which will report the diagnostic.
             */
            DiagEngine &engine;
            /*!
             \brief The diagnostic level.
             */
            DiagLevel level;
            /*!
             \brief The text describing the diagnostic.
             */
            std::string text;
            /*!
             \brief The reference to the source file this diagnostic is about.
             */
            src::SrcLoc *srcloc;
            /*!
             \brief List of the params received.
             */
            std::vector<std::string> params;
            /*!
             \brief The number of params needed.
             */
            unsigned paramsNeeded = 0;
            
        public:
            Diagnostic(DiagEngine &engine, DiagLevel level, std::string text, src::SrcLoc *srcloc = nullptr);
            
            Diagnostic(DiagEngine &engine, Diagnostic &diagnostic) : Diagnostic(engine, diagnostic.level, diagnostic.text, diagnostic.srcloc) {
                params = diagnostic.params;
            }
            
            ~Diagnostic() {
                if (srcloc) delete srcloc;
            }
            
            
            inline DiagLevel getLevel() const {
                return level;
            }
            
            inline const std::string &getText() const {
                return text;
            }
            
            inline const std::vector<std::string> &getParams() const {
                return params;
            }
            
            inline src::SrcLoc *getSrcLoc() const {
                return srcloc;
            }
            
            /*!
             \brief Adds the given param to the params list. When diagnostic is printed, "%i" in the text will be replaced with \c params[i] .
             \param printIfComplete If the given param is the last param needed to complete the diagnostic, the diagnostic will be printed if this param is \c true (by default).
             */
            Diagnostic &addParam(std::string param, bool printIfComplete = true);
            /*!
             \brief Returns whether the diagnostics has received all the params, and is ready to be reported.
             */
            inline bool isComplete() const {
                return params.size() >= paramsNeeded;
            }
            
            inline Diagnostic &operator <<(std::string param) {
                return addParam(param);
            }
            
            inline Diagnostic &operator <<(const char *param) {
                return addParam(param);
            }
            
            template<typename T> inline Diagnostic &operator <<(T param) {
                return addParam(std::to_string(param));
            }
        };
        
        /*!
         \brief Struct containing the status for the diagnostics reported in a specific part of the compilation session.
         */
        class DiagnosticsReport {
            friend class DiagEngine;
            
            /*!
             \brief The diagnostics contained in this report.
             */
            std::vector<Diagnostic *> diags;
            /*!
             \brief The number of the errors contained in this report.
             */
            unsigned errorsReported = 0;
            /*!
             \brief The number of the warnings contained in this report.
             */
            unsigned warningsReported = 0;
            /*!
             \brief The number of the notes contained in this report.
             */
            unsigned notesReported = 0;
            
        public:
            DiagnosticsReport() {  }
            
            /*!
             \brief Returns a vector with the diagnostics contained in this report.
             \note If the diagnostic report is given to different diagnostic engines, diagnostics from different engines may be stored. Only use \c passToEngine()
             */
            inline const std::vector<Diagnostic *> &getDiags() const {
                return diags;
            }
            /*!
             \brief Returns the number of the errors container in this report.
             */
            inline unsigned getErrors() const {
                return errorsReported;
            }
            /*!
             \brief Returns the number of the warnings container in this report.
             */
            inline unsigned getWarnings() const {
                return warningsReported;
            }
            /*!
             \brief Returns the number of the notes container in this report.
             */
            inline unsigned getNotes() const {
                return notesReported;
            }
            
            /*!
             \brief Adds the given diagnostic to the report.
             */
            void addDiagnostic(Diagnostic &diagnostic);
            /*!
             \brief Copies and reports all the diagnostics contained in this report to the given engine. Any complete diagnostic passed to the given engine will be printed by it if \c printIfComplete is \c true .
             */
            void passToEngine(DiagEngine &engine, bool printIfComplete = true);
            
        };
        
        /*!
         \brief Interface for an arbitrary output the compiler can report warnings and errors to.
         */
        class DiagEngine {
            friend class Diagnostic;
            friend class DiagnosticsReport;
            
            /*!
             \brief Options for diagnostics reporting.
             */
            opts::DiagnosticsOptions &opts;
            /*!
             \brief The output this diag engine will send complete diagnostics to.
             */
            DiagOutput &output;
            /*!
             \brief The number of errors reported by the engine.
             */
            unsigned reportedErrors = 0;
            /*!
             \brief The number of warnings reported by the engine.
             */
            unsigned reportedWarnings = 0;
            /*!
             \brief The number of notes reported by the engine.
             */
            unsigned reportedNotes = 0;
            /*!
             \brief The report that will keep the incoming diagnostics.
             */
            std::vector<DiagnosticsReport *> reports;
            
        public:
            /*!
             \brief Initializes the diagnostics engine with the driver for the compilation to be diagnosed.
             */
            DiagEngine(opts::DiagnosticsOptions &opts, DiagOutput &output) : opts(opts), output(output) {  }
            
            /*!
             \brief Writes a diagnostic to the diagnostics engine to write it on the designed output.
             \param ID The ID for the diagnostic associated with the text to display to the user.
             \param srcloc The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            Diagnostic &reportDiag(DiagLevel level, DiagID ID, src::SrcLoc *srcloc = nullptr);
            
            Diagnostic &reportDiag(Diagnostic *diag);
            /*!
             \brief Overload method to write a custom diagnostic to the diagnostics engine to write it on the designed output.
             \param text The custom text to display to the user. Params from %0 to %9 may be inserted and replaced with the \c << operator. 
             \param srcloc The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            Diagnostic &reportCustomDiag(DiagLevel level, std::string text, src::SrcLoc *srcloc = nullptr);
            /*!
             \brief Overload method to write an error diagnostic to the diagnostics engine to write it on the designed output.
             \param ID The ID for the diagnostic associated with the text to display to the user.
             \param srcloc The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            
            inline Diagnostic &reportError(DiagID ID, src::SrcLoc *srcloc = nullptr) {
                return reportDiag(diag::Error, ID, srcloc);
            }
            /*!
             \brief Overload method to write a warning diagnostic to the diagnostics engine to write it on the designed output.
             \param ID The ID for the diagnostic associated with the text to display to the user.
             \param srcloc The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            inline Diagnostic &reportWarning(DiagID ID, src::SrcLoc *srcloc = nullptr) {
                return reportDiag(diag::Warning, ID, srcloc);
            }
            /*!
             \brief Overload method to write a note diagnostic to the diagnostics engine to write it on the designed output.
             \param ID The ID for the diagnostic associated with the text to display to the user.
             \param srcloc The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            inline Diagnostic &reportNote(DiagID ID, src::SrcLoc *srcloc = nullptr) {
                return reportDiag(diag::Note, ID, srcloc);
            }
            
            /*!
             \brief Returns the number of errors reported to the engine.
             */
            inline unsigned getErrorCount() const {
                return reportedErrors;
            }
            /*!
             \brief Returns the number of warnings reported to the engine.
             */
            inline unsigned getWarningCount() const {
                return reportedWarnings;
            }
            
            /*!
             \brief Adds the given diagnostics report to update with the future diagnostics reported to the engine.
             */
            void addReport(DiagnosticsReport &report);
            /*!
             \brief Removes the given diagnostics report so it will not be updated anymore by the engine.
             */
            void removeReport(DiagnosticsReport &report);
            /*!
             \brief Prints the given report to the diagnostics engine stream.
             \note This does not print the diagnostics, but only the errors and warnings count.
             */
            void printReport(DiagnosticsReport &report);
            /*!
             \brief Prints the given report to the diagnostics engine stream and removes it from updating reports.
             \note This does not print the diagnostics, but only the errors and warnings count.
             */
            void printAndRemoveReport(DiagnosticsReport &report);
            
        private:
            /*!
             \brief Sends the given diagnostic to the engine output.
             */
            void sendDiag(Diagnostic &diag);
            
        };
        
    }
}

#endif
