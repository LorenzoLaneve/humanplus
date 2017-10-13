// => src/analyzers/lexer/control.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/lexer/lexer.h>

using namespace hpc;

bool lexer::LexerInstance::eof() {
    return getCurrentToken() == TokenEOF;
}

bool lexer::LexerInstance::escape() {
    while (!isDelimiter(getCurrentToken()) && getCurrentToken() != '}') {
        if (eof()) return false;
        
        if (getCurrentToken() == '{') {
            unsigned int braces = 1;
            while (braces) {
                switch (getNextToken()) {
                    case '{':
                        braces++;
                        break;
                    case '}':
                        braces--;
                        break;
                }
            }
            
            break;
        }
        getNextToken();
    }
    getNextToken();
    return true;
}
