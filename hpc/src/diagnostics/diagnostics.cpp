// => src/diagnostics/diagnostics.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/diagnostics/diagnostics.h>
#include <hpc/utils/printers.h>
#include <hpc/utils/strings.h>
#include <hpc/utils/opts.h>

#include <map>
#include <string>
#include <sstream>
#include <algorithm>

#define HUMAN_PLUS_ERROR_COUNTER_LIMIT 10

using namespace hpc;

static std::map<diag::DiagID, std::string> diagStrings = {
    { diag::Unknown,
        "unknown error occured" },
    
    { diag::ArgumentToOptionIsMissing,
        "argument to '%0' is missing (expected 1 value)" },
    { diag::MoreArgumentsToOptionAreMissing,
        "argument to '%0' is missing (expected %1 values)" },
    { diag::UnknownArgument,
        "unknown argument '%0'" },
    { diag::UnsupportedOption,
        "unsupported option '%0'" },
    { diag::NoInputFiles,
        "no input files" },
    { diag::NoSuchFileOrDirectory,
        "no such file or directory: '%0'" },
    { diag::LinkerFailedWithExitCode,
        "linker failed with exit code %0 (use -v to see invocation)" },
    { diag::InvalidOptionValueInFlag,
        "invalid option value '%0' in '%1'" },
    { diag::EmittingLLVMWhileLinking,
        "-emit-llvm cannot be used when linking" },
    { diag::LLVMFatalError,
        "fatal error by LLVM: %0" },
    { diag::ABICreationFailed,
        "unable to recreate target ABI" },
    { diag::UnableToCreateLLVMTarget,
        "unable to create target: %0" },
    { diag::ErrorOpeningFile,
        "error reading '%0'" },
    
    { diag::InvalidSuffixOnIntegerLiteral,
        "invalid suffix on integer constant" },
    { diag::InvalidSuffixOnFloatingPointLiteral,
        "invalid suffix on floating constant" },
    { diag::ValueTooLargeForAnyNumberType,
        "number literal is too large to be represented in any number type" },
    { diag::InvalidCharacterLiteral,
        "Invalid character sequence in literal here." },
    { diag::InvalidDigitInBinaryConstant,
        "invalid digit '%0' in binary constant" },
    { diag::InvalidDigitInOctalConstant,
        "invalid digit '%0' in octal constant" },
    
    { diag::ExpectedUnqualifiedIdentifier,
        "expected unqualified-id" },
    { diag::ExpectedOpenBrace,
        "expected '{'" },
    { diag::ExpectedDelimiterAfterTopLevel,
        "expected ';' after top level declarator" },
    { diag::ExpectedTokenFunction,
        "expected 'function' after function attributes" },
    { diag::InvalidArgumentList,
        "expected ')'" },
    { diag::ExpectedTupleForFunctionDeclaration,
        "expected '(' for function declaration" },
    { diag::ExpectedTokenBeAfterLetDeclaration,
        "expected 'be' for 'let' declaration" },
    { diag::ExpectedExpression,
        "expected expression" },
    { diag::ExpectedClosedTuple,
        "expected ')'" },
    { diag::ExpectedDelimiterAfterLocalLet,
        "expected ';' after 'let' declaration" },
    { diag::ExpectedDelimiterAfterReturn,
        "expected ';' after return statement" },
    { diag::ExpectedDelimiterAfterExpression,
        "expected ';' after expression" },
    { diag::ConflictingUnsignedQualifier,
        "cannot combine with previous 'signed' declaration" },
    { diag::ConflictingSignedQualifier,
        "cannot combine with previous 'unsigned' declaration" },
    { diag::ExpectedOfAfterAlias,
        "expected 'of' after 'alias'" },
    { diag::ExpectedEqualAfterAlias,
        "expected '=' or 'of' after alias declaration" },
    { diag::ExpectedType,
        "expected type" },
    { diag::ExpectedWhileOrUntilAfterDo,
        "expected 'while' or 'until' after 'do' statement" },
    { diag::ExpectedDelimiterAfterDoWhile,
        "expected ';' after 'do/while' statement" },
    { diag::ExpectedDelimiterAfterDoUntil,
        "expected ';' after 'do/until' statement" },
    { diag::ExpectedDelimiterAfterBreak,
        "expected ';' after 'break' statement" },
    { diag::ExpectedDelimiterAfterContinue,
        "expected ';' after 'continue' statement" },
    { diag::ExpectedMemberIdentifier,
        "expected member identifier after access operation" },
    { diag::UnexpectedEOF,
        "unexpected end of file" },
    
    { diag::IncompatibleTypesInBinary,
        "invalid operands to binary expression (%0 and %1)" },
    { diag::UnknownTypeName,
        "unknown type name '%0'" },
    { diag::RedefinitionOfLocalVariable,
        "redefinition of '%0'" },
    { diag::RedefinitionOfLocalVarWithDifType,
        "redefinition of '%0' with a different type: %1 vs %2" },
    { diag::NoMemberInNameSpace,
        "no member named '%0' in namespace '%1'" },
    { diag::NoTypeInNameSpace,
        "no type named '%0' in namespace '%1'" },
    { diag::UndeclaredIdentifier,
        "use of undeclared identifier '%0'" },
    { diag::NotNestedComponent,
        "'%0' is not a class or namespace" },
    { diag::NoViableConversion,
        "no viable conversion from %0 to %1" },
    { diag::ExpressionNotAssignable,
        "expression is not assignable" },
    { diag::FunctionOverloadDoesNotExist,
        "no matching function for call to '%0'" },
    { diag::FunctionCallIsAmbiguous,
        "call to '%0' is ambiguous" },
    { diag::ExpressionIsNotEvaluable,
        "value of type %0 is not contextually convertible to a 'bool'" },
    { diag::ControlReachesEndOfNonVoidFunction,
        "control may reach end of non-void function" },
    { diag::ReturnForVoidFunctionHasValue,
        "void function '%0' should not return a value" },
    { diag::ReturnForNonVoidFunctionNeedsValue,
        "non-void function '%0' should return a value" },
    { diag::NoViableConversionInReturn,
        "no viable conversion from returned value of type %0 to function return type %1" },
    { diag::RedefinitionOfTypeAlias,
        "redefinition of type alias" },
    { diag::RedefinitionOfTypeAliasWithDifType,
        "redefinition of type alias with different types (%0 vs %1)" },
    { diag::InitValueForMoreThanOneVariable,
        "initialization value in a declaration of more than one variable" },
    { diag::TypeInferencedVariableNeedsInitVal,
        "variables declared with type inference must have an initial value" },
    { diag::TypeInferenceVariableWithNullLiteral,
        "variables declared with type inference cannot be initialized with an explicit null pointer" },
    { diag::RedefinitionOfType,
        "definition of type '%0' conflicts with the previously declared type with the same name" },
    { diag::NoObjectMemberInType,
        "no member named '%0' in type %1" },
    { diag::InvalidExpressionToUnaryOperation,
        "invalid argument type %0 to unary expression" },
    { diag::BreakNotInBreakableStatement,
        "'break' statement not in a switch or loop statement" },
    { diag::ContinueNotInContinuableStatement,
        "'continue' statement not in a loop statement" },
    { diag::TypeCannotBeSignedOrUnsigned,
        "%0 cannot be signed or unsigned" },

    { diag::CandidateFunction,
        "candidate function" },
    { diag::PreviousVariableDeclarationWasHere,
        "previous declaration of '%0' was here" },
    { diag::PreviousTypeDeclarationWasHere,
        "'%0' declared here" }
};


diag::Diagnostic::Diagnostic(DiagEngine &engine, DiagLevel level, std::string text, src::SrcLoc *tkref)
: engine(engine), level(level), text(text), tkref(tkref) {
    // A little algorithm searching for the %i with the max i. e.g.: if %8 if the max index contained in text, then we asssume the diagnostic needs 9 params.
    int maxIndex = -1;
    size_t pos = 0;
    
    while ((pos = text.find("%", pos)) != std::string::npos) {
        if (++pos < text.length()) {
            int index = text[pos] - '0';
            assert(0 <= index && index <= 9 && "Invalid param placeholder.");
            
            if (index >= maxIndex) {
                maxIndex = index;
            }
            
        }
    }
    
    paramsNeeded = maxIndex + 1;
}

diag::Diagnostic &diag::Diagnostic::addParam(std::string param, bool printIfComplete) {
    if (!isComplete()) {
        params.push_back(param);
        
        if (printIfComplete && isComplete()) {
            engine.sendDiag(*this);
        }
    }
    return *this;
}


void diag::DiagnosticsReport::addDiagnostic(Diagnostic &diagnostic) {
    diags.push_back(&diagnostic);
    
    switch (diagnostic.level) {
        case Error:
            errorsReported++;
            break;
        case Warning:
            warningsReported++;
            break;
        case Note:
            notesReported++;
            break;
        default: break;
    }
}

void diag::DiagnosticsReport::passToEngine(DiagEngine &engine, bool printIfComplete) {
    for (Diagnostic *diag : diags)
        engine.reportDiag(diag);
}


diag::Diagnostic &diag::DiagEngine::reportDiag(DiagLevel level, DiagID ID, src::SrcLoc *tkref) {
    return reportCustomDiag(level, diagStrings[ID], tkref);
}

diag::Diagnostic &diag::DiagEngine::reportDiag(Diagnostic *diag) {
    if (diag->isComplete()) {
        sendDiag(*diag);
    }
    
    for (DiagnosticsReport *report : reports) report->addDiagnostic(*diag);
    
    return *diag;
}

diag::Diagnostic &diag::DiagEngine::reportCustomDiag(DiagLevel level, std::string text, src::SrcLoc *tkref) {
    return reportDiag(new Diagnostic(*this, level, text, tkref));
}

void diag::DiagEngine::addReport(DiagnosticsReport &report) {
    reports.push_back(&report);
}

void diag::DiagEngine::removeReport(DiagnosticsReport &report) {
    reports.erase(std::remove(reports.begin(), reports.end(), &report), reports.end());
}

void diag::DiagEngine::printReport(DiagnosticsReport &report) {
    unsigned errs = report.errorsReported;
    unsigned warns = report.warningsReported;
    
    if (errs || warns) {
        llvm::outs() << "\n";
        if (errs) {
            if (errs > 1) {
                llvm::outs() << errs << " errors";
            } else {
                llvm::outs() << errs << " error";
            }
            
            if (warns) {
                llvm::outs() << " and ";
            }
        }
        
        if (warns) {
            if (warns > 1) {
                llvm::outs() << warns << " warnings";
            } else {
                llvm::outs() << warns << " warning";
            }
        }
        
        llvm::outs() << " generated.\n";
    }
}

void diag::DiagEngine::printAndRemoveReport(DiagnosticsReport &report) {
    removeReport(report);
    printReport(report);
}

void diag::DiagEngine::sendDiag(Diagnostic &diag) {
    assert(diag.isComplete() && "diag is not complete.");
    
    output.handleDiag(diag);
    
    switch (diag.getLevel()) {
        case Error:
            reportedErrors++;
            break;
        case Warning:
            reportedWarnings++;
            break;
        case Note:
            reportedNotes++;
            break;
        default: break;
    }
}
