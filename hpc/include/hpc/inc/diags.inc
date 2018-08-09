// => hpc/inc/diags.inc
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __predef_diag
#error "__predef_diag not defined before including diags.inc"
#endif


__predef_diag(Unknown,                              "unknown error occured")

__predef_diag(ArgumentToOptionIsMissing,            "argument to '%0' is missing (expected 1 value)")
__predef_diag(MoreArgumentsToOptionAreMissing,      "argument to '%0' is missing (expected %1 values)")
__predef_diag(UnknownArgument,                      "unknown argument '%0'")
__predef_diag(UnsupportedOption,                    "unsupported option '%0'")
__predef_diag(NoInputFiles,                         "no input files")
__predef_diag(NoSuchFileOrDirectory,                "no such file or directory: '%0'")
__predef_diag(LinkerFailedWithExitCode,             "linker failed with exit code %0 (use -v to see invocation)")
__predef_diag(InvalidOptionValueInFlag,             "invalid option value '%0' in '%1'")
__predef_diag(EmittingLLVMWhileLinking,             "-emit-llvm cannot be used when linking")
__predef_diag(LLVMFatalError,                       "fatal error by LLVM: %0")
__predef_diag(ABICreationFailed,                    "unable to recreate target ABI")
__predef_diag(UnableToCreateLLVMTarget,             "unable to create target: %0")
__predef_diag(ErrorOpeningFile,                     "error reading '%0'")

__predef_diag(ExponentHasNoDigits,                  "exponent has no digits")
__predef_diag(InvalidCharacterLiteral,              "Invalid character sequence in literal here.")
__predef_diag(InvalidDigitInBinaryConstant,         "invalid digit '%0' in binary constant")
__predef_diag(InvalidDigitInOctalConstant,          "invalid digit '%0' in octal constant")

__predef_diag(ExpectedUnqualifiedIdentifier,        "expected unqualified-id")
__predef_diag(ExpectedOpenBrace,                    "expected '{'")
__predef_diag(ExpectedDelimiterAfterTopLevel,       "expected ';' after top level declarator")
__predef_diag(ExpectedTokenFunction,                "expected 'function' after function attributes")
__predef_diag(InvalidArgumentList,                  "expected ')'")
__predef_diag(ExpectedTupleForFunctionDeclaration,  "expected '(' for function declaration")
__predef_diag(ExpectedTokenBeAfterLetDeclaration,   "expected 'be' for 'let' declaration")
__predef_diag(ExpectedExpression,                   "expected expression")
__predef_diag(ExpectedClosedTuple,                  "expected ')'")
__predef_diag(ExpectedDelimiterAfterLocalLet,       "expected ';' after 'let' declaration")
__predef_diag(ExpectedDelimiterAfterReturn,         "expected ';' after return statement")
__predef_diag(ExpectedDelimiterAfterExpression,     "expected ';' after expression")
__predef_diag(ConflictingUnsignedQualifier,         "cannot combine with previous 'signed' declaration")
__predef_diag(ConflictingSignedQualifier,           "cannot combine with previous 'unsigned' declaration")
__predef_diag(ExpectedOfAfterAlias,                 "expected 'of' after 'alias'")
__predef_diag(ExpectedEqualAfterAlias,              "expected '=' or 'of' after alias declaration")
__predef_diag(ExpectedType,                         "expected type")
__predef_diag(ExpectedWhileOrUntilAfterDo,          "expected 'while' or 'until' after 'do' statement")
__predef_diag(ExpectedDelimiterAfterDoWhile,        "expected ';' after 'do/while' statement")
__predef_diag(ExpectedDelimiterAfterDoUntil,        "expected ';' after 'do/until' statement")
__predef_diag(ExpectedDelimiterAfterBreak,          "expected ';' after 'break' statement")
__predef_diag(ExpectedDelimiterAfterContinue,       "expected ';' after 'continue' statement")
__predef_diag(ExpectedMemberIdentifier,             "expected member identifier after access operation")
__predef_diag(UnexpectedEOF,                        "unexpected end of file")

__predef_diag(IncompatibleTypesInBinary,            "invalid operands to binary expression (%0 and %1)")
__predef_diag(UnknownTypeName,                      "unknown type name '%0'")
__predef_diag(RedefinitionOfLocalVariable,          "redefinition of '%0'")
__predef_diag(RedefinitionOfLocalVarWithDifType,    "redefinition of '%0' with a different type: %1 vs %2")
__predef_diag(NoMemberInNameSpace,                  "no member named '%0' in namespace '%1'")
__predef_diag(NoTypeInNameSpace,                    "no type named '%0' in namespace '%1'")
__predef_diag(UndeclaredIdentifier,                 "use of undeclared identifier '%0'")
__predef_diag(NotNestedComponent,                   "'%0' is not a class or namespace")
__predef_diag(NoViableConversion,                   "no viable conversion from %0 to %1")
__predef_diag(ExpressionNotAssignable,              "expression is not assignable")
__predef_diag(FunctionOverloadDoesNotExist,         "no matching function for call to '%0'")
__predef_diag(FunctionCallIsAmbiguous,              "call to '%0' is ambiguous")
__predef_diag(ExpressionIsNotEvaluable,             "value of type %0 is not contextually convertible to a 'bool'")
__predef_diag(ControlReachesEndOfNonVoidFunction,   "control may reach end of non-void function")
__predef_diag(ReturnForVoidFunctionHasValue,        "void function '%0' should not return a value")
__predef_diag(ReturnForNonVoidFunctionNeedsValue,   "non-void function '%0' should return a value")
__predef_diag(NoViableConversionInReturn,           "no viable conversion from returned value of type %0 to function return type %1")
__predef_diag(RedefinitionOfTypeAlias,              "redefinition of type alias")
__predef_diag(RedefinitionOfTypeAliasWithDifType,   "redefinition of type alias with different types (%0 vs %1)")
__predef_diag(InitValueForMoreThanOneVariable,      "initialization value in a declaration of more than one variable")
__predef_diag(TypeInferencedVariableNeedsInitVal,   "variables declared with type inference must have an initial value")
__predef_diag(TypeInferenceVariableWithNullLiteral, "variables declared with type inference cannot be initialized with an explicit null pointer")
__predef_diag(RedefinitionOfType,                   "definition of type '%0' conflicts with the previously declared type with the same name")
__predef_diag(NoObjectMemberInType,                 "no member named '%0' in type %1")
__predef_diag(InvalidExpressionToUnaryOperation,    "invalid argument type %0 to unary expression")
__predef_diag(BreakNotInBreakableStatement,         "'break' statement not in a switch or loop statement")
__predef_diag(ContinueNotInContinuableStatement,    "'continue' statement not in a loop statement")
__predef_diag(TypeCannotBeSignedOrUnsigned,         "%0 cannot be signed or unsigned")

__predef_diag(CandidateFunction,                    "candidate function")
__predef_diag(PreviousVariableDeclarationWasHere,   "previous declaration of '%0' was here")
__predef_diag(PreviousTypeDeclarationWasHere,       "'%0' declared here")

