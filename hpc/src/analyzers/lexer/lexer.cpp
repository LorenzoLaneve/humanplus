// => src/analyzers/lexer/lexer.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/lexer/lexer.h>


using namespace hpc;

lexer::LexerInstance::LexerInstance(diag::DiagEngine &diags, src::SourceFile &source) : diags(diags), source(source) {
    open();
}

lexer::LexerInstance::~LexerInstance() {
    close();
}

void lexer::LexerInstance::open() {
    reader = new SourceReader(source);
}

void lexer::LexerInstance::close() {
    if (reader) {
        delete reader;
        reader = nullptr;
    }
}

bool lexer::LexerInstance::escape() {
    while (getCurrentToken() != ';' && getCurrentToken() != '}') {
        // FIXME not only colons may be delimiters. A function to determine whether a given char is a statement delimiter should be provided.
        
        if (eof()) return false;
        
        if (getCurrentToken() == '{') {
            unsigned long braces = 1;
            while (braces) {
                switch (getNextToken().getTokenType()) {
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
