// => src/analyzers/lexer/bufferer.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
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
