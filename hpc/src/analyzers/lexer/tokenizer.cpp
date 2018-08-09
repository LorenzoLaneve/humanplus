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
#include <string>

#define emit_tokenref() currentRef = *sourcefile->getCaret(); if (srcloc) *srcloc = currentRef; resetFetchCount()
#define tokenref_len(x) currentRef.length = x; if (srcloc) *srcloc = currentRef

#define lowerize(x) (x = tolower(x))

using namespace hpc;

lexer::Token &lexer::LexerInstance::getLastToken() {
    assert(lastToken && "Less than two tokens have been read.");
    return *lastToken;
}

lexer::Token &lexer::LexerInstance::getCurrentToken() {
    assert(currentToken && "No tokens have been read.");
    return *currentToken;
}

lexer::Token &lexer::LexerInstance::getNextToken() {
    delete lastToken;
    lastToken = currentToken;
    
    currentToken = new Token();
    readNewToken(*currentToken);
    return *currentToken;
}


void lexer::LexerInstance::readNewToken(Token &newToken) {
    // Skip any whitespace/line feed/anything else that isn't a visible character.
    while (isspace(lastChar)) {
        lastChar = fetch();
    }
    
    if (newToken.location) { // if there's already a SrcLoc, then we should delete it.
        delete newToken.location;
    }
    
    // Take the caret to keep track of the token location, and reset the fetch counter to measure its length.
    newToken.location = getSourceReader().getCaret();
    getSourceReader().resetFetchCounter();
    
    if (isalpha(lastChar) || lastChar == '_') { // read identifier
        std::ostringstream idstream;
        idstream << (char)lastChar; // add the already read character...
        
        // ...and any alphanumeric and underscore character.
        while (isalnum(lastChar = fetch()) || lastChar == '_') idstream << (char)lastChar;
        
        // use fetch counter for identifier length.
        newToken.location->length = getSourceReader().getFetchCount();
        
        std::string identifier = idstream.str();
        
        checkKeyword(newToken, identifier);
        return;
    }
    
    if (isdigit(lastChar) || lastChar == '.') { // read number literal
        if (!checkNumberLiteral(newToken)) {
            // checkNumberLiteral will try to set a number literal to the token.
            // if false is returned, then there is only the point and not a literal, so a \c MemberAccess should be set.
            
            newToken.type = Token::MemberAccess;
        }
        return;
    }
    
    if (lastChar == '\'') { // read character literal
        checkCharLiteral(newToken);
        return;
    }
    
    if (lastChar == '"') { // read string literal
        checkStringLiteral(newToken);
        return;
    }
    
    
    // OTHER TOKEN TYPES
    switch (lastChar) {
        case '<': {
            switch (lastChar = fetch()) {
                case '=': {
                    newToken.type = Token::OperatorLowerEqual;
                    break;
                }
                case '>': {
                    newToken.type = Token::OperatorNotEqual;
                    // FIXME should be different from != at token level to allow different overloadings.
                    break;
                }
                case '<': {
                    newToken.type = Token::OperatorLeftShift;
                    break;
                }
                case '-': {
                    newToken.type = Token::Pointer;
                    // FIXME should be different from 'pointer' at token level.
                    break;
                }
                default: {
                    newToken.type = Token::OperatorLower;
                    return;
                }
            }
            break;
        }
        case '>': {
            switch (lastChar = fetch()) {
                case '=': {
                    newToken.type = Token::OperatorGreaterEqual;
                    break;
                }
                case '>': {
                    newToken.type = Token::OperatorRightShift;
                    break;
                }
                default: {
                    newToken.type = Token::OperatorGreater;
                    return;
                }
            }
            break;
        }
        case '=': {
            switch (lastChar = fetch()) {
                case '=': {
                    newToken.type = Token::OperatorEqual;
                    break;
                }
                default: {
                    newToken.type = Token::OperatorAssign;
                    return;
                }
            }
            break;
        }
        case '!': {
            switch (lastChar = fetch()) {
                case '=': {
                    newToken.type = Token::OperatorNotEqual;
                    break;
                }
                default: {
                    newToken.type = Token::OperatorExclMark;
                    return;
                }
            }
            break;
        }
        case '+': {
            switch (lastChar = fetch()) {
                case '=': {
                    newToken.type = OP_ATTACH_ASSIGNMENT(Token::OperatorPlus);
                    break;
                }
                default: {
                    newToken.type = Token::OperatorPlus;
                    return;
                }
            }
            break;
        }
        case '-': {
            switch (lastChar = fetch()) {
                case '=': {
                    newToken.type = OP_ATTACH_ASSIGNMENT(Token::OperatorMinus);
                    break;
                }
                default: {
                    newToken.type = Token::OperatorMinus;
                    return;
                }
            }
            break;
        }
        case '*': {
            switch (lastChar = fetch()) {
                case '=': {
                    newToken.type = OP_ATTACH_ASSIGNMENT(Token::OperatorMultiply);
                    break;
                }
                default: {
                    newToken.type = Token::OperatorMultiply;
                    return;
                }
            }
            break;
        }
        case '/': {
            switch (lastChar = fetch()) {
                case '/': {
                    ignoreInlineComment();
                    readNewToken(newToken);
                    return;
                }
                case '*': {
                    ignoreMultilineComment();
                    readNewToken(newToken);
                    return;
                }
                
                    
                case '=': {
                    newToken.type = OP_ATTACH_ASSIGNMENT(Token::OperatorDivide);
                    break;
                }
                default: {
                    newToken.type = Token::OperatorDivide;
                    return;
                }
            }
            break;
        }
        case '%': {
            switch (lastChar = fetch()) {
                case '=': {
                    newToken.type = OP_ATTACH_ASSIGNMENT(Token::OperatorRemainder);
                    break;
                }
                default: {
                    newToken.type = Token::OperatorRemainder;
                    return;
                }
            }
            break;
        }
        case ':': {
            switch (lastChar = fetch()) {
                case ':': {
                    newToken.type = Token::NameSpaceBrowser;
                    break;
                }
                default: {
                    newToken.type = ':';
                    return;
                }
            }
            break;
        }
        case EOF: {
            newToken.type = Token::EndOfFile;
            return;
        }
        default: {
            newToken.type = lastChar;
            break;
        }
    }
    
    lastChar = fetch();
}

bool lexer::LexerInstance::checkKeyword(Token &newToken, std::string identifier) {
    newToken.type = llvm::StringSwitch<Token::Type>(identifier)
    
    // constructs keywords
    .Case("function",       Token::Function)
    .Case("namespace",      Token::NameSpace)
    .Case("class",          Token::Class)
    .Case("protocol",       Token::Protocol)
    
    // statement keywords
    .Case("let",            Token::Let)
    .Case("be",             Token::Be)
    .Case("alias",          Token::Alias)
    .Case("if",             Token::If)
    .Case("then",           Token::Then)
    .Case("else",           Token::Else)
    .Case("do",             Token::Do)
    .Case("while",          Token::While)
    .Case("until",          Token::Until)
    .Case("for",            Token::For)
    .Case("switch",         Token::Switch)
    .Case("break",          Token::Break)
    .Case("continue",       Token::Continue)
    .Case("return",         Token::Return)
    
    // operator keywords
    .Case("and",            Token::And)
    .Case("or",             Token::Or)
    .Case("as",             Token::As)
    
    // literals
    .Case("true",           Token::True)
    .Case("false",          Token::False)
    .Case("yes",            Token::True)
    .Case("no",             Token::False)
    .Case("nothing",        Token::Null)
    .Case("null",           Token::Null)
    .Case("nil",            Token::Null)
    
    // built-in type names
    .Case("void",           Token::TypeVoid)
    .Case("bool",           Token::TypeBool)
    .Case("boolean",        Token::TypeBool)
    .Case("char",           Token::TypeChar)
    .Case("character",      Token::TypeChar)
    .Case("byte",           Token::TypeByte)
    .Case("short",          Token::TypeShort)
    .Case("int",            Token::TypeInteger)
    .Case("integer",        Token::TypeInteger)
    .Case("long",           Token::TypeLong)
    .Case("float",          Token::TypeFloat)
    .Case("single",         Token::TypeFloat)
    .Case("double",         Token::TypeDouble)
    
    // type qualifiers and modifiers
    .Case("unsigned",       Token::Unsigned)
    .Case("signed",         Token::Signed)
    .Case("immutable",      Token::Constant)
    .Case("constant",       Token::Constant)
    .Case("pointer",        Token::Pointer)
    .Case("nostalgic",      Token::Nostalgic)
    .Case("returns",        Token::Returns)
    //.Case("returning",    Token::Returns)
    .Case("extends",        Token::Extends)
    
    // unqualified identifier
    .Default(Token::Identifier);
    
    if (newToken.type == Token::Identifier) { // If that's an unqual id then save it and return false as expected.
        newToken.identifier = new std::string(identifier);
        return false;
    }
    return true;
}

bool lexer::LexerInstance::ignoreInlineComment() {
    while ((lastChar = fetch()) != '\n') {
        if (lastChar == EOF) {
            return false;
        }
    }
    return true;
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

