// => src/analyzers/lexer/literals.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/utils/numbers.h>
#include <hpc/diagnostics/diagnostics.h>

#include <llvm/ADT/StringSwitch.h>

using namespace hpc;

int lexer::LexerInstance::putDecimalConstant(std::string numberString, lexer::token_ty suffixtype, source::SrcLoc *literalRef) {
    switch (suffixtype) {
        case TokenIntegerLiteral:
            if (util::dec_toi(numberString, currentInteger))
                return TokenIntegerLiteral;
        case TokenUnsignedIntegerLiteral:
            if (util::dec_toui(numberString, currentUnsignedInteger))
                return TokenUnsignedIntegerLiteral;
        case TokenLongLiteral:
            if (util::dec_tol(numberString, currentLong))
                return TokenLongLiteral;
        case TokenUnsignedLongLiteral:
            if (util::dec_toul(numberString, currentUnsignedLong))
                return TokenUnsignedLongLiteral;
        case TokenFloatLiteral:
            if (util::dec_tof(numberString, currentFloat))
                return TokenFloatLiteral;
        case TokenDoubleLiteral:
            if (util::dec_tod(numberString, currentDouble))
                return TokenDoubleLiteral;
    }
    
    diags.reportError(diag::ValueTooLargeForAnyNumberType, literalRef);
    currentDouble = 0;
    return TokenDoubleLiteral;
}

int lexer::LexerInstance::putHexadecimalConstant(std::string numberString, lexer::token_ty suffixtype, source::SrcLoc *literalRef) {
    switch (suffixtype) {
        case TokenIntegerLiteral:
            if (util::hex_toi(numberString, currentInteger))
                return TokenIntegerLiteral;
        case TokenUnsignedIntegerLiteral:
            if (util::hex_toui(numberString, currentUnsignedInteger))
                return TokenUnsignedIntegerLiteral;
        case TokenLongLiteral:
            if (util::hex_tol(numberString, currentLong))
                return TokenLongLiteral;
        case TokenUnsignedLongLiteral:
        case TokenFloatLiteral: // FIXME
        case TokenDoubleLiteral:
            if (util::hex_toul(numberString, currentUnsignedLong))
                return TokenUnsignedLongLiteral;
    }
    
    diags.reportError(diag::ValueTooLargeForAnyNumberType, literalRef);
    currentDouble = 0;
    return TokenDoubleLiteral;
}

int lexer::LexerInstance::putBinaryConstant(std::string numberString, lexer::token_ty suffixtype, source::SrcLoc *literalRef) {
    switch (suffixtype) {
        case lexer::TokenIntegerLiteral:
            if (util::bin_toi(numberString, currentInteger))
                return lexer::TokenIntegerLiteral;
        case lexer::TokenUnsignedIntegerLiteral:
            if (util::bin_toui(numberString, currentUnsignedInteger))
                return lexer::TokenUnsignedIntegerLiteral;
        case lexer::TokenLongLiteral:
            if (util::bin_tol(numberString, currentLong))
                return lexer::TokenLongLiteral;
        case lexer::TokenUnsignedLongLiteral:
        case lexer::TokenFloatLiteral: // FIXME
        case lexer::TokenDoubleLiteral:
            if (util::bin_toul(numberString, currentUnsignedLong))
                return lexer::TokenUnsignedLongLiteral;
    }
    
    diags.reportError(diag::ValueTooLargeForAnyNumberType, literalRef);
    currentDouble = 0;
    return lexer::TokenDoubleLiteral;
}

int lexer::LexerInstance::putOctalConstant(std::string numberString, lexer::token_ty suffixtype, source::SrcLoc *literalRef) {
    switch (suffixtype) {
        case TokenIntegerLiteral:
            if (util::oct_toi(numberString, currentInteger))
                return TokenIntegerLiteral;
        case TokenUnsignedIntegerLiteral:
            if (util::oct_toui(numberString, currentUnsignedInteger))
                return TokenUnsignedIntegerLiteral;
        case TokenLongLiteral:
            if (util::oct_tol(numberString, currentLong))
                return TokenLongLiteral;
        case TokenUnsignedLongLiteral:
        case TokenFloatLiteral: // FIXME
        case TokenDoubleLiteral:
            if (util::oct_toul(numberString, currentUnsignedLong))
                return TokenUnsignedLongLiteral;
    }
    
    diags.reportError(diag::ValueTooLargeForAnyNumberType, literalRef);
    currentDouble = 0;
    return TokenDoubleLiteral;
}

