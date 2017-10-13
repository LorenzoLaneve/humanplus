//
//  diagnostics.h
//  HumanPlus
//
//  Created by Lore on 29/05/16.
//  Copyright © 2016 Lore. All rights reserved.
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
         \brief Values indicating types of diagnostics that can be shown to the user.
         */
        typedef enum {
            /*!
             \brief An unknown error occurred.
             */
            Unknown                             = 0,
            
            //
            // Frontend errors
            //
            /*!
             \brief An option that should have an argument does not specify it.
             \param 0 A string containing the flag
             */
            ArgumentToOptionIsMissing           = 1,
            /*!
             \brief An option that should have more than one argument does not specify them.
             \param 0 A string containing the flag
             \param 1 A string containing the number of expected arguments.
             */
            MoreArgumentsToOptionAreMissing     = 2,
            /*!
             \brief A flag is not recognized by the compiler.
             \param 0 A string containing the unrecognized flag
             */
            UnknownArgument                     = 3,
            /*!
             \brief An option is not supported by the compiler.
             \param 0 A string containing the unrecognized option
             */
            UnsupportedOption                   = 4,
            /*!
             \brief No input files were specified in the command line.
             */
            NoInputFiles                        = 5,
            /*!
             \brief There is no file or directory with the given name.
             \param 0 The file name of the non-existing file.
             */
            NoSuchFileOrDirectory               = 6,
            /*!
             \brief The linker failed the linkage process with an exit status != \c 0
             */
            LinkerFailedWithExitCode            = 7,
            /*!
             \brief The option value given with a flag is not valid for the flag.
             \param 0 The value
             \param 1 The complete flag
             */
            InvalidOptionValueInFlag            = 8,
            /*!
             \brief -emit-llvm is specified, meaning the compiler will emit output as LLVM IR, which cannot be linked.
             */
            EmittingLLVMWhileLinking            = 9,
            /*!
             \brief LLVM backend thrown a fatal error.
             */
            LLVMFatalError                      = 10,
            /*!
             \brief Frontend failed to create the requested target ABI.
             */
            ABICreationFailed                   = 11,
            /*!
             \brief Frontend failed to create the requested LLVM target machine.
             */
            UnableToCreateLLVMTarget            = 12,
            /*!
             \brief Error opening a file
             \param 0 The filename
             */
            ErrorOpeningFile                    = 13,
            
            //
            // Lexical errors (1xx)
            //
            
            /*!
             \brief The lexer found an integer number token with an unknown suffix.
             */
            InvalidSuffixOnIntegerLiteral       = 101,
            /*!
             \brief The lexer found an floating point number token with an unknown suffix.
             */
            InvalidSuffixOnFloatingPointLiteral = 102,
            /*!
             \brief The lexer found an integer number token which can't be expressed in any number type.
             */
            ValueTooLargeForAnyNumberType       = 103,
            /*!
             \brief The lexer found a character literal containing an unknown escape sequence or that represents more than one character.
             */
            InvalidCharacterLiteral             = 104,
            /*!
             \brief The lexer found a digit in the binary integer literal which is not a valid binary digit.
             \param 0 The invalid digit
             */
            InvalidDigitInBinaryConstant        = 105,
            /*!
             \brief The lexer found a digit in the octal integer literal which is not a valid octal digit.
             \param 0 The invalid digit
             */
            InvalidDigitInOctalConstant         = 106,
            
            
            //
            // Syntax errors (2xx)
            //
            
            /*!
             \brief The parser has not found a non-keyword identifier, as expected.
             */
            ExpectedUnqualifiedIdentifier       = 201,
            /*!
             \brief The parser has not found an open brace \c {, as expected.
             */
            ExpectedOpenBrace                   = 202,
            /*!
             \brief The parser has not found a delimiter as end of a top level declaration.
             */
            ExpectedDelimiterAfterTopLevel      = 203,
            /*!
             \brief The parser has not found the \c function keyword, as expected.
             */
            ExpectedTokenFunction               = 204,
            /*!
             \brief The parser has not found a valid way to continue an argument list.
             */
            InvalidArgumentList                 = 205,
            /*!
             \brief The parser has not found the open tuple \c ( for function declaration arguments, as expected.
             */
            ExpectedTupleForFunctionDeclaration = 206,
            /*!
             \brief The parser has not found the \c be keyword after identifiers specified with \c let, as expected.
             */
            ExpectedTokenBeAfterLetDeclaration  = 207,
            /*!
             \brief The parser has not found an expression, as expected.
             */
            ExpectedExpression                  = 208,
            /*!
             \brief The parser has not found a closing tuple \c }, as expected.
             */
            ExpectedClosedTuple                 = 209,
            /*!
             \brief The parser has not found a delimiter as end of a local \c let declaration.
             */
            ExpectedDelimiterAfterLocalLet      = 210,
            /*!
             \brief The parser has not found a delimiter as end of a \c return statement.
             */
            ExpectedDelimiterAfterReturn        = 211,
            /*!
             \brief The parser has not found a delimiter as end of an expression statement.
             */
            ExpectedDelimiterAfterExpression    = 212,
            /*!
             \brief The parser has found an \c unsigned qualifier after a \c signed qualifier.
             */
            ConflictingUnsignedQualifier        = 213,
            /*!
             \brief The parser has found a \c signed qualifier after an \c unsigned qualifier.
             */
            ConflictingSignedQualifier          = 214,
            /*!
             \brief The parser has not found an \c of keyword after the \c alias keyword, as expected.
             */
            ExpectedOfAfterAlias                = 215,
            /*!
             \brief The parser has not found a '=' or an \c or keyword in the \c alias declaration, as expected.
             */
            ExpectedEqualAfterAlias             = 216,
            /*!
             \brief The parser has not found a valid type, as expected.
             */
            ExpectedType                        = 217,
            /*!
             \brief The parser has not found a \c while or an \c until after the \c do block, as expected.
             */
            ExpectedWhileOrUntilAfterDo         = 218,
            /*!
             \brief The parser has not found a delimiter as end of a \c do-while statement.
             */
            ExpectedDelimiterAfterDoWhile       = 219,
            /*!
             \brief The parser has not found a delimiter as end of a \c do-until statement.
             */
            ExpectedDelimiterAfterDoUntil       = 220,
            /*!
             \brief The parser has not found a delimiter as end of a \c break statement.
             */
            ExpectedDelimiterAfterBreak         = 221,
            /*!
             \brief The parser has not found a delimiter as end of a \c continue statement.
             */
            ExpectedDelimiterAfterContinue      = 222,
            /*!
             \brief The parser has not found a member identifier after an access operation, as expected.
             */
            ExpectedMemberIdentifier            = 223,
            /*!
             \brief The lexer finished the file in the middle of a parsing process.
             */
            UnexpectedEOF                       = 224,
            
            
            //
            // Semantics errors (3xx)
            //
            
            /*!
             \brief Operands under a binary expression have incompatible types
             \param 0 A string describing the left-hand side operand type.
             \param 0 A string describing the right-hand side operand type.
             */
            IncompatibleTypesInBinary           = 301,
            /*!
             \brief A symbol found as type doesn't match any existing type
             \param 0 A string containing the symbol.
             */
            UnknownTypeName                     = 302,
            /*!
             \brief There is a new variable with an identifier that is used by previously declared variable.
             \param 0 The variable identifier
             */
            RedefinitionOfLocalVariable         = 303,
            /*!
             \brief There is a new variable with an identifier that is used by previously declared variable, with a different type.
             \param 0 The variable identifier
             \param 1 The current variable type
             \param 2 The previously declared variable type
             */
            RedefinitionOfLocalVarWithDifType   = 304,
            /*!
             \brief A symbol is trying to access a non-existing member in a specific namespace.
             \param 0 The non-existing namespace member identifier
             \param 1 The namespace member the validator was searching
             */
            NoMemberInNameSpace                   = 305,
            /*!
             \brief A symbol is trying to access a non-existing type in a specific namespace.
             \param 0 The non-existing type identifier
             \param 1 The namespace member the validator was searching
             */
            NoTypeInNameSpace                     = 306,
            /*!
             \brief A symbol is using an identifier which actually hasn't been declared.
             \param 0 The identifier
             */
            UndeclaredIdentifier                = 307,
            /*!
             \brief A symbol is refering to a class or namespace with an identifier, but the associated entity either does not exist or is a class or namespace.
             \param 0 The identifier of the member
             */
            NotNestedComponent                  = 308,
            /*!
             \brief An implicit cast is attempted in order to assign the value to a new entity, but the types are incompatible.
             \param 0 The type of the value to be converted
             \param 1 The type of the entity the value had to be assigned to
             */
            NoViableConversion                  = 309,
            /*!
             \brief In the left-hand side of an assignment there is an expression which cannot be assigned.
             */
            ExpressionNotAssignable             = 310,
            /*!
             \brief A call to a function is attempted, but the arguments passed to che call cannot be casted to any overload for that function.
             \param 0 The identifier of the function
             */
            FunctionOverloadDoesNotExist        = 311,
            /*!
             \brief A call to a function is attempted, but there is more than one candidate function that can be linked to this call.
             \param 0 The identifier of the function
             */
            FunctionCallIsAmbiguous             = 312,
            /*!
             \brief An explicit cast to a boolean is attempted, but the expression is in a type which cannot be evaluated.
             */
            ExpressionIsNotEvaluable            = 313,
            /*!
             \brief The function contains statements that can lead to the end of function without encountering any \c return statement.
             */
            ControlReachesEndOfNonVoidFunction  = 314,
            /*!
             \brief A \c return statement in a function declared as \c void has an expression to be returned.
             \param 0 The function identifier
             */
            ReturnForVoidFunctionHasValue       = 315,
            /*!
             \brief A \c return statement in a function that returns a value does not have an expression to be returned.
             \param 0 The function identifier
             */
            ReturnForNonVoidFunctionNeedsValue  = 316,
            /*!
             \brief A \c return statement in a function has an expression which type cannot be converted to the function return type.
             \param 0 A string describing the returned expression type
             \param 1 A string describing the function return type
             */
            NoViableConversionInReturn          = 317,
            /*!
             \brief A \c type alias is declared with an identifier that has already been declared.
             */
            RedefinitionOfTypeAlias             = 318,
            /*!
             \brief A \c type alias is declared with an identifier that has already been declared, with a different type.
             \param 0 The current alias original type
             \param 1 The previously declared alias original type
             */
            RedefinitionOfTypeAliasWithDifType  = 319,
            /*!
             \brief An initial value is specified in a \c let declaration which declares more than one variable.
             */
            InitValueForMoreThanOneVariable     = 320,
            /*!
             \brief A variable with type inference does not have an initial value with an inferable type.
             */
            TypeInferencedVariableNeedsInitVal  = 321,
            /*!
             \brief A variable with type inference has a \c null literal as initial value.
             */
            TypeInferenceVariableWithNullLiteral = 322,
            /*!
             \brief A type is declared with an identifier that has already been declared in the same scope.
             \param 0 The type identifier
             */
            RedefinitionOfType                   = 323,
            /*!
             \brief An access to an undeclared member of an object is attempted.
             \param 0 The member identifier
             \param 1 The identifier of the class of the object
             */
            NoObjectMemberInType                 = 324,
            /*!
             \brief The expression in the unary operation is not supported by the operator.
             \param 0 The type identifier
             */
            InvalidExpressionToUnaryOperation    = 325,
            /*!
             \brief A \c break statement is found, but no breakable statements are registered in the local scope.
             */
            BreakNotInBreakableStatement         = 326,
            /*!
             \brief A \c continue statement is found, but no continuable statements are registered in the local scope.
             */
            ContinueNotInContinuableStatement    = 327,
            /*!
             \brief The parsed type has 'unsigned' or 'signed' qualifier but it's not an integer type.
             */
            TypeCannotBeSignedOrUnsigned         = 328,
            
            
            //
            // Warnings (1xxx)
            //
            
            // Nothing yet.
            
            
            //
            // Notes (2xxx)
            //
            
            /*!
             \brief Note that indicates a candidate function. It usually follows a \c FunctionCallIsAmbiguous error.
             */
            CandidateFunction                   = 2001,
            /*!
             \brief Note that indicates a previous declaration of an already declared variable. It usually follows a \c RedefinitionOfLocalVariable error.
             \param 0 The variable identifier
             */
            PreviousVariableDeclarationWasHere  = 2002,
            /*!
             \brief Note that indicates a previous declaration of an already declared type alias. It usually follows a \c RedefinitionOfTypeAlias error.
             \param 0 The type alias identifier
             */
            PreviousTypeDeclarationWasHere      = 2003
            
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
            source::TokenRef *tkref;
            /*!
             \brief List of the params received.
             */
            std::vector<std::string> params;
            /*!
             \brief The number of params needed.
             */
            unsigned paramsNeeded = 0;
            
        public:
            Diagnostic(DiagEngine &engine, DiagLevel level, std::string text, source::TokenRef *tkref = nullptr);
            
            Diagnostic(DiagEngine &engine, Diagnostic &diagnostic) : Diagnostic(engine, diagnostic.level, diagnostic.text, diagnostic.tkref) {
                params = diagnostic.params;
            }
            
            ~Diagnostic() {
                if (tkref) delete tkref;
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
            
            inline source::TokenRef *getTokenRef() const {
                return tkref;
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
             \param tkref The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            Diagnostic &reportDiag(DiagLevel level, DiagID ID, source::TokenRef *tkref = nullptr);
            
            Diagnostic &reportDiag(Diagnostic *diag);
            /*!
             \brief Overload method to write a custom diagnostic to the diagnostics engine to write it on the designed output.
             \param text The custom text to display to the user. Params from %0 to %9 may be inserted and replaced with the \c << operator. 
             \param tkref The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            Diagnostic &reportCustomDiag(DiagLevel level, std::string text, source::TokenRef *tkref = nullptr);
            /*!
             \brief Overload method to write an error diagnostic to the diagnostics engine to write it on the designed output.
             \param ID The ID for the diagnostic associated with the text to display to the user.
             \param tkref The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            
            inline Diagnostic &reportError(DiagID ID, source::TokenRef *tkref = nullptr) {
                return reportDiag(diag::Error, ID, tkref);
            }
            /*!
             \brief Overload method to write a warning diagnostic to the diagnostics engine to write it on the designed output.
             \param ID The ID for the diagnostic associated with the text to display to the user.
             \param tkref The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            inline Diagnostic &reportWarning(DiagID ID, source::TokenRef *tkref = nullptr) {
                return reportDiag(diag::Warning, ID, tkref);
            }
            /*!
             \brief Overload method to write a note diagnostic to the diagnostics engine to write it on the designed output.
             \param ID The ID for the diagnostic associated with the text to display to the user.
             \param tkref The reference in the source to the token involved in the diagnostic.
             \param params An array of strings which will be replaced in the displaying text.
             \see \c DiagID values to know what to put in \c params vector.
             */
            inline Diagnostic &reportNote(DiagID ID, source::TokenRef *tkref = nullptr) {
                return reportDiag(diag::Note, ID, tkref);
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
