//
//  lexer.cpp
//  Human Plus
//
//  Created by Lore on 08/03/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
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
