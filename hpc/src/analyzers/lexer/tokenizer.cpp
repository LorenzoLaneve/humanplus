// => src/analyzers/lexer/tokenizer.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/analyzers/syntax/operators.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/utils/numbers.h>

#include <llvm/ADT/StringSwitch.h>

#include <sstream>

#define emit_tokenref() currentRef = *sourcefile->getCaret(); if (tkref) *tkref = currentRef; resetFetchCount()
#define tokenref_len(x) currentRef.length = x; if (tkref) *tkref = currentRef

#define lowerize(x) (x = tolower(x))

using namespace hpc;

std::string lexer::LexerInstance::getCurrentIdentifier() {
    return currentIdentifier;
}

lexer::token_ty lexer::LexerInstance::getNewToken(source::SrcLoc *tkref) {
    while (isspace(lastChar)) lastChar = fetch();
    
    emit_tokenref();
    if (isalpha(lastChar) || lastChar == '_') {
        currentIdentifier = lastChar;
        while (isalnum(lastChar = fetch()) || lastChar == '_') currentIdentifier += lastChar;
        
        if (currentIdentifier == "programmer") { // commento
            while ((lastChar = fetch()) != '\n') if (lastChar == EOF) return TokenEOF;
            return getNextToken(tkref);
        }
        tokenref_len(getFetchCount());
        
        return llvm::StringSwitch<token_ty>(currentIdentifier)

        // constructs keywords
        .Case("function",       TokenFunction)
        .Case("namespace",        TokenNameSpace)
        .Case("class",          TokenClass)
        .Case("protocol",       TokenProtocol)

        // statement keywords
        .Case("let",            TokenLet)
        .Case("be",             TokenBe)
        .Case("alias",          TokenAlias)
        .Case("if",             TokenIf)
        .Case("then",           TokenThen)
        .Case("else",           TokenElse)
        .Case("do",             TokenDo)
        .Case("while",          TokenWhile)
        .Case("until",          TokenUntil)
        .Case("for",            TokenFor)
        .Case("switch",         TokenSwitch)
        .Case("break",          TokenBreak)
        .Case("continue",       TokenContinue)
        .Case("return",         TokenReturnStatement)
        
        // operator keywords
        .Case("and",            TokenOperatorLogicalAnd)
        .Case("or",             TokenOperatorLogicalOr)
        .Case("as",             TokenAs)
        
        // literals
        .Case("true",           TokenTrue)
        .Case("false",          TokenFalse)
        .Case("yes",            TokenTrue)
        .Case("no",             TokenFalse)
        .Case("nothing",        TokenNull)
        .Case("null",           TokenNull)
        .Case("nil",            TokenNull)
        
        // built-in type names
        .Case("void",           TokenTypeVoid)
        .Case("bool",           TokenTypeBool)
        .Case("boolean",        TokenTypeBool)
        .Case("char",           TokenTypeChar)
        .Case("character",      TokenTypeChar)
        .Case("byte",           TokenTypeByte)
        .Case("short",          TokenTypeShort)
        .Case("int",            TokenTypeInteger)
        .Case("integer",        TokenTypeInteger)
        .Case("long",           TokenTypeLong)
        .Case("float",          TokenTypeFloat)
        .Case("single",         TokenTypeFloat)
        .Case("double",         TokenTypeDouble)
        
        // type qualifiers and modifiers
        .Case("unsigned",       TokenUnsigned)
        .Case("signed",         TokenSigned)
        .Case("immutable",      TokenConstant)
        .Case("constant",       TokenConstant)
        .Case("pointer",        TokenPointer)
        .Case("nostalgic",      TokenNostalgic)
        .Case("returns",        TokenReturnQualifier)
        //.Case("returning",      TokenReturnQualifier)
        .Case("extends",        TokenExtends)
        
        // unqualified identifier
        .Default(TokenIdentifier);
    }
    currentIdentifier = "";
    
    if (isdigit(lastChar) || lastChar == '.') {
        lexer::constant_type ltype = lexer::decimalConstant;
        
        bool hasFP = lastChar == '.';
        std::string numberString;
        numberString += lastChar;
        
        if (!isdigit(lastChar = fetch()) && hasFP) {
            return TokenOperatorMemberAccess;
        } else if (numberString[0] == '0' && lastChar == 'x') { // Hexadecimal
            numberString = "";
            
            ltype = lexer::hexadecimalConstant;
            while (isdigit(lastChar = fetch()) || ('a' <= lowerize(lastChar) && lastChar <= 'f')) {
                numberString += lastChar;
            }
            
        } else if (numberString[0] == '0' && lastChar == 'b') { // Binary
            numberString = "";
            ltype = lexer::binaryConstant;
            
            bool isValid = true;
            while (isdigit(lastChar = fetch())) {
                if (lastChar >= '2') {
                    diags.reportError(diag::InvalidDigitInBinaryConstant, sourcefile->getCaret()) << lastChar - '0';
                    
                    isValid = false;
                }
                
                numberString += lastChar;
            }
            if (!isValid) numberString = "0";
            
        } else if (numberString[0] == '0' && lastChar == 'o') { // Octal
            numberString = "";
            ltype = lexer::octalConstant;
            
            bool isValid = true;
            while (isdigit(lastChar = fetch())) {
                if (lastChar >= '8') {
                    diags.reportError(diag::InvalidDigitInOctalConstant, sourcefile->getCaret()) << lastChar - '0';
                    isValid = false;
                }
                
                numberString += lastChar;
            }
            if (!isValid) numberString = "0";
            
        } else { // Decimal
            
            ltype = lexer::decimalConstant;
            while (isdigit(lastChar) || (!hasFP && lastChar == '.')) {
                numberString += lastChar;
                hasFP = hasFP || lastChar == '.';
                lastChar = fetch();
            }
            
        }
        
        tokenref_len(getFetchCount());
        
        lexer::token_ty suffixtype = hasFP ? TokenDoubleLiteral : TokenIntegerLiteral;
        if (isalpha(lastChar) || lastChar == '_') {
            suffixtype = 0;
            
            src::SrcLoc suffixref;
            if (getNextToken(&suffixref) == TokenIdentifier) {
                std::string suffix = currentIdentifier;
                if (hasFP) {
                    if (suffix == "f")
                        suffixtype = lexer::TokenFloatLiteral;
                    
                    if (!suffixtype) {
                        diags.reportError(diag::InvalidSuffixOnFloatingPointLiteral, &suffixref);
                        currentDouble = 0;
                        return lexer::TokenDoubleLiteral;
                    }
                } else {
                    suffixtype = llvm::StringSwitch<lexer::token_ty>(currentIdentifier)
                    .Case("u", lexer::TokenUnsignedIntegerLiteral)
                    .Case("l", lexer::TokenLongLiteral)
                    .Case("ul", lexer::TokenUnsignedLongLiteral)
                    .Case("f", lexer::TokenFloatLiteral)
                    .Default(suffixtype);
                    
                    if (!suffixtype) {
                        diags.reportError(diag::InvalidSuffixOnIntegerLiteral, &suffixref);
                        currentUnsignedLong = 0;
                        return lexer::TokenUnsignedLongLiteral;
                    }
                }
            } else if (hasFP) {
                diags.reportError(diag::InvalidSuffixOnFloatingPointLiteral, &suffixref);
                currentDouble = 0;
                return lexer::TokenDoubleLiteral;
            } else {
                diags.reportError(diag::InvalidSuffixOnIntegerLiteral, &suffixref);
                currentUnsignedLong = 0;
                return lexer::TokenUnsignedLongLiteral;
            }
        }
        
        switch (ltype) {
            case lexer::decimalConstant:
                return putDecimalConstant(numberString, suffixtype, &currentRef);
            case lexer::hexadecimalConstant:
                return putHexadecimalConstant(numberString, suffixtype, &currentRef);
            case lexer::binaryConstant:
                return putBinaryConstant(numberString, suffixtype, &currentRef);
            case lexer::octalConstant:
                return putOctalConstant(numberString, suffixtype, &currentRef);
                
            default:
                llvm_unreachable("Invalid literal type");
        }
    }
    
    if (lastChar == '\'') {
        if ((lastChar = fetch()) == '\'') {
            diags.reportError(diag::InvalidCharacterLiteral);
            currentCharacter = 0;
            return TokenCharacterLiteral;
        }
        currentCharacter = getASCIIChar('\'');
        
        if ((lastChar = fetch()) != '\'') {
            diags.reportError(diag::InvalidCharacterLiteral);
        }
        tokenref_len(getFetchCount());
        
        lastChar = fetch();
        return TokenCharacterLiteral;
    }
    
    if (lastChar == '"') {
        std::ostringstream strliteral("");
        
        while ((lastChar = fetch()) != '"') {
            rt::utf7_char_ty sc = getASCIIChar('"');
            if (sc == EOF) return TokenEOF;
            strliteral << sc;
        }
        tokenref_len(getFetchCount());
        
        currentString = strliteral.str();
        lastChar = fetch();
        return TokenStringLiteral;
    }
    
    switch (lastChar) {
        case '<': {
            if ((lastChar = fetch()) == '=') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return TokenOperatorLowerEqual;
            } else if (lastChar == '>') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return TokenOperatorNotEqual;
            } else if (lastChar == '<') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return TokenOperatorLeftShift;
            } else if (lastChar == '-') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return TokenPointer;
            } else {
                tokenref_len(1);
                return TokenOperatorLower;
            }
            break;
        }
        case '>': {
            if ((lastChar = fetch()) == '=') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return TokenOperatorGreaterEqual;
            } else if (lastChar == '>') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return TokenOperatorRightShift;
            } else return TokenOperatorGreater;
            break;
        }
        case '=': {
            if ((lastChar = fetch()) == '=') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return TokenOperatorEqual;
            } else return TokenOperatorAssign;
            break;
        }
        case '!': {
            if ((lastChar = fetch()) == '=') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return TokenOperatorNotEqual;
            } else {
                tokenref_len(1);
                return TokenOperatorExclMark;
            }
            break;
        }
        case '+': {
            if ((lastChar = fetch()) == '=') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return __operator_compound_assignment+TokenOperatorPlus;
            } else {
                tokenref_len(1);
                return TokenOperatorPlus;
            }
            break;
        }
        case '-': {
            if ((lastChar = fetch()) == '=') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return __operator_compound_assignment+TokenOperatorMinus;
            } else {
                tokenref_len(1);
                return TokenOperatorMinus;
            }
            break;
        }
        case '*': {
            if ((lastChar = fetch()) == '=') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return __operator_compound_assignment+TokenOperatorMultiply;
            } else {
                tokenref_len(1);
                return TokenOperatorMultiply;
            }
            break;
        }
        case '/': {
            if ((lastChar = fetch()) == '/') { // inline comments
                while ((lastChar = fetch()) != '\n') if (lastChar == EOF) return TokenEOF;
                return getNextToken();
            } else if (lastChar == '*') { // multiline comments
                ignoreMultilineComment();
                return getNextToken();
            }
            
            if ((lastChar = fetch()) == '=') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return __operator_compound_assignment+TokenOperatorDivide;
            } else {
                tokenref_len(1);
                return TokenOperatorDivide;
            }
            break;
        }
        case '%': {
            if ((lastChar = fetch()) == '=') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return __operator_compound_assignment+TokenOperatorRemainder;
            } else {
                tokenref_len(1);
                return TokenOperatorRemainder;
            }
            break;
        }
        case ':': {
            if ((lastChar = fetch()) == ':') {
                tokenref_len(getFetchCount());
                lastChar = fetch();
                return TokenNameSpaceBrowser;
            } else {
                tokenref_len(1);
                return ':';
            }
        }
    }
    
    tokenref_len(1);
    if (lastChar == EOF) return TokenEOF;
    
    int returning = lastChar;
    lastChar = fetch();
    return returning;
}

rt::utf7_char_ty lexer::LexerInstance::getASCIIChar(src::sourcechar quote) {
    if (lastChar == '\\') { // TODO octal number character literal
        switch (lastChar = fetch()) {
            case '\\':
            case '\?':
                return lastChar;
            case '0':
                return '\0';
            case 'a':
                return '\a';
            case 'b':
                return '\b';
            case 'f':
                return '\f';
            case 'n':
                return '\n';
            case 'r':
                return '\r';
            case 't':
                return '\t';
            case 'v':
                return '\v';
            case EOF:
                return EOF;
        }
    }
    return lastChar;
}

bool lexer::LexerInstance::ignoreMultilineComment() {
    do {
        
        while ((lastChar = fetch()) != '*') {
            if (lastChar == EOF) return false;
            
            if (lastChar == '/') {
                if ((lastChar = fetch()) == '*') {
                    if (!ignoreMultilineComment()) return false;
                }
            }
        }
        
    } while ((lastChar = fetch()) != '/');
    
    lastChar = fetch();
    return true;
}

void lexer::LexerInstance::resetTokenizer() {
    lastChar = ' ';
    
    currentRef = {};
    currentToken = 0;
    
    lastRef = {};
    lastToken = 0;
    
    currentIdentifier = "";
}

