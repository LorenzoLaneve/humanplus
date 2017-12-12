// => src/analyzers/lexer/tokens.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/lexer/tokens.h>

using namespace hpc;

lexer::Token::~Token() {
    // Free all the optional data.
    
    if (location)       delete location;
    if (identifier)     delete identifier;
    if (intLiteral)     delete intLiteral;
    if (uintLiteral)    delete uintLiteral;
    if (longLiteral)    delete longLiteral;
    if (ulongLiteral)   delete ulongLiteral;
    if (floatLiteral)   delete floatLiteral;
    if (doubleLiteral)  delete doubleLiteral;
}
