//
//  bufferer.cpp
//  HumanPlus
//
//  Created by Lore on 10/05/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/analyzers/lexer/lexer.h>

using namespace hpc;

source::sourcechar lexer::LexerInstance::fetch() {
    assert(sourcefile && "No Source File object bound to the lexer.");
    fetchCount++;
    return sourcefile->fetchChar();
}

unsigned long lexer::LexerInstance::getFetchCount() {
    return fetchCount;
}

void lexer::LexerInstance::resetFetchCount(bool alreadyStarted) {
    fetchCount = alreadyStarted ? 1 : 0;
}
