//
//  grammar.cpp
//  HumanPlus
//
//  Created by Lore on 10/05/16.
//  Copyright © 2016 Lore. All rights reserved.
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
