// => src/analyzers/lexer/grammar.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/lexer/lexer.h>

using namespace hpc;

void lexer::LexerInstance::ignoreArticleIfAny() {
    if (getCurrentToken() == TokenIdentifier && (currentIdentifier == "a" || currentIdentifier == "an")) getNextToken();
}

void lexer::LexerInstance::ignoreRelativePronounsIfAny() {
    if (getCurrentToken() == TokenIdentifier && (currentIdentifier == "that" || currentIdentifier == "which")) getNextToken();
}

void lexer::LexerInstance::ignoreAndIfAny() {
    if (getCurrentToken() == TokenIdentifier && currentIdentifier == "and") getNextToken();
}

bool lexer::isDelimiter(char c) {
    return c == ';';
}
