//
//  control.cpp
//  HumanPlus
//
//  Created by Lore on 10/05/16.
//  Copyright © 2016 Lore. All rights reserved.
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
