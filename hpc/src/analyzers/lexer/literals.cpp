// => src/analyzers/lexer/literals.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/analyzers/sources.h>
#include <hpc/analyzers/syntax/literals.h>
#include <hpc/utils/numbers.h>
#include <hpc/diagnostics/diagnostics.h>

#include <llvm/ADT/StringSwitch.h>

#include <sstream>

#define INVALID_CHAR_FOR_LITERAL -1
#define VALID_SUFFIX -2

using namespace hpc;

static bool isValidDigit(lexer::fchar_t c, syntax::NumberLiteral::Base base) {
    if (!isdigit(c)) {
        if (base == syntax::NumberLiteral::Base16) {
            c = tolower(c);
            return 'a' <= c && c <= 'f';
        }
        return false;
    }
    return c < base;
}

static int getHexValue(lexer::fchar_t c) {
    if (isdigit(c)) {
        return c - '0';
    }
    if ('a' <= c && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}


bool lexer::LexerInstance::checkNumberLiteral(Token &newToken) {
    
    bool hasFP = (lastChar == '.'); // Whether the floating point has been already specified.
    syntax::NumberLiteral::Base base = syntax::NumberLiteral::Base10;
    
    std::ostringstream literalString;
    
    if (hasFP) {
        // We are starting with a dot, so we can assume that if we are in front of a literal, that will definitely be a floating point literal, which can only be base 10. FIXME
        
        if (!isdigit(lastChar = fetch())) {
            return false;
        }
        
        literalString << '.';
        
    } else if (lastChar == '0') {
        // if it starts with 0, we should expect prefixes for other bases.
        
        switch (tolower(lastChar = fetch())) {
            case 'b':
                base = syntax::NumberLiteral::Base2;
                lastChar = fetch();
                break;
            case 'o':
                base = syntax::NumberLiteral::Base8;
                lastChar = fetch();
                break;
            case 'x':
                base = syntax::NumberLiteral::Base16;
                lastChar = fetch();
                break;
            default:
                // if none of the specified prefixes are found, we'll assume to have an octal constant.
                // Any non-digit will be read as suffix. If the literal ends here, we'll have a 0, which is treated as octal constant.
                base = syntax::NumberLiteral::Base8;
                break;
        }
    }
    
    if (hasFP) { // FIXME we assume that any floating point literal is base 10.
        base = syntax::NumberLiteral::Base10;
    }
    
    while (isValidDigit(lastChar, base) || lastChar == '.') {
        if (lastChar != '\'') { // ignoring quotes between digits.
            if (lastChar == '.') {
                if (hasFP || base != syntax::NumberLiteral::Base10) {
                    // if this is not the first floating point found, then we stop and read this as suffix.
                    // FIXME we also stop if this is not a decimal literal, as we're not supporting
                    //       non-decimal floating point literals as of now.
                    
                    lastChar = fetch(); // take new char first.
                    break;
                }
                hasFP = true;
            }
            
            literalString << (char)lastChar;
        }
        lastChar = fetch();
    }
    
    // any exponent will be added to the literal string, and not treated as suffix.
    if (tolower(lastChar) == 'e') {
        src::SrcLoc *caret = getSourceReader().getCaret();
        // Pointer to the 'e' character.
        
        literalString << 'e' << (char)(lastChar = fetch());
        
        if (lastChar == '+' || lastChar == '-') {
            literalString << (char)lastChar;
            lastChar = fetch();
        }
        
        unsigned long digitCount;
        for (digitCount = 0; isdigit(lastChar = fetch()); digitCount++) {
            literalString << (char)lastChar;
        }
        
        
        if (!digitCount) { // no digits for exponent, throw error.
            diags.reportError(diag::ExponentHasNoDigits, caret);
        }
        
        delete caret;
    }
    
    // parsing the suffix: [a-zA-Z0-9._]
    std::ostringstream suffixString;
    
    while (isalnum(lastChar) || lastChar == '.' || lastChar == '_') {
        suffixString << (char)lastChar;
        lastChar = fetch();
    }
    
    newToken.type = Token::NumberLiteral;
    newToken.numberLiteral = new syntax::NumberLiteral(literalString.str(), suffixString.str(), base);
    
    return true;
}




bool lexer::LexerInstance::checkCharEscapeSequence(syntax::CharData &cdata) {
    syntax::CharSeqFormat format = syntax::FormatAsIs;
    syntax::CharValue val;
    if (lastChar != '\\') { // escaped sequence
        
        val = 0;
        switch (lastChar = fetch()) {
            case 'x':
            case 'u':
            case 'U': {
                int N = 0; // number of hex digits
                
                if (lastChar == 'x') {
                    format = syntax::FormatHex8;
                    N = 2;
                } else
                if (lastChar == 'u') {
                    format = syntax::FormatHex16;
                    N = 4;
                } else
                if (lastChar == 'U') {
                    format = syntax::FormatHex32;
                    N = 8;
                }
                
                for (int i = 0; i < N; i++) {
                    lastChar = fetch();
                    
                    val <<= 4;
                    if (isValidDigit(lastChar, syntax::NumberLiteral::Base16)) {
                        val |= getHexValue(lastChar);
                    } else {
                        diags.reportError(diag::ExpectedHexadecimalDigit, getCaret());
                        lastChar = fetch();
                        return false;
                    }
                }
                
                break;
            }
            
#define __escape_seq(toRead, value) \
            case toRead: { \
                val = value; \
                break; \
            }
                
#include "/Archie/git/humanplus/hpc/include/hpc/inc/syntax/escapes.inc"
#undef __escape_seq
                
            default: {
                diags.reportError(diag::InvalidEscapeSequenceInChar, getCaret());
                lastChar = fetch();
                return false;
            }
        }

    } else {
        // not an escape sequence.
        val = lastChar;
    }
    
    lastChar = fetch();
    
    cdata = { val, format };
    return true;
}

void lexer::LexerInstance::checkCharLiteral(Token &newToken) {
    syntax::CharData cdata;
    
    lastChar = fetch(); // eat the quote.
    if (checkCharEscapeSequence(cdata)) {
        if (lastChar != '\'') {
            diags.reportError(diag::MultiCharConstant, getCaret());
        }
    } else {
        while (lastChar != '\'' && lastChar != EOF) { // go ahead till the second quote...
            lastChar = fetch();
        }
        
        lastChar = fetch(); //...and drop it
    }
    
    newToken.type = Token::CharLiteral;
    newToken.charLiteral = new syntax::CharacterLiteral(cdata, "");
    // FIXME suffixes
}

void lexer::LexerInstance::checkStringLiteral(Token &newToken) {
    syntax::CharData cdata;
    
    syntax::StringLiteral *stringLiteral = new syntax::StringLiteral();
    while ((lastChar = fetch()) != '\"') {
        if (lastChar == EOF) {
            break;
        }
        
        if (checkCharEscapeSequence(cdata)) {
            stringLiteral->add(cdata);
        } else {
            // FIXME string literal error should be reported by checkCharEscapeSequence.
            while (lastChar != '\"' && lastChar != EOF) { // go ahead till the second quote...
                lastChar = fetch();
            }
            
            lastChar = fetch(); //...and drop it
            break;
        }
    }
    
    newToken.type = Token::StringLiteral;
    newToken.stringLiteral = stringLiteral;
}




















