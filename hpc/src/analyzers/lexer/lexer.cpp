// => src/analyzers/lexer/lexer.cpp
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

#include <sstream>

using namespace hpc;

lexer::LexerInstance::LexerInstance(diag::DiagEngine &diags, source::SourceFile *sourceFile) : diags(diags), sourcefile(sourceFile) {
    resetFetchCount(false);
    resetTokenizer();
}

lexer::LexerInstance::~LexerInstance() {
    unbind();
}

source::SourceFile *lexer::LexerInstance::getSourceFile() {
    return sourcefile;
}

void lexer::LexerInstance::unbind() {
    sourcefile->close();
    sourcefile = nullptr;
}
