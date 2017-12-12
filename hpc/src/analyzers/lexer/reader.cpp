// => src/analyzers/lexer/reader.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/lexer/reader.h>

#include <stdlib.h>

#define BUFFER_SIZE 8192

using namespace hpc;

lexer::SourceReader::SourceReader(source::SourceFile &source) : source(source) {
    fp = fopen(source.getFileName().c_str(), "r");
    
    buffer = (unsigned char *)malloc(BUFFER_SIZE * sizeof(unsigned char));
    leftChars = 0;
    
    caret = { &source };
}

lexer::SourceReader::~SourceReader() {
    close();
    free(buffer);
}

void lexer::SourceReader::fillUpBuffer() {
    fread(buffer, sizeof(unsigned char), BUFFER_SIZE, fp);
    leftChars = BUFFER_SIZE;
    nextbuf = buffer;
}

lexer::fchar_t lexer::SourceReader::readChar() {
    if (leftChars <= 0) {
        fillUpBuffer();
    }
    leftChars--;
    return *nextbuf++;
}

lexer::fchar_t lexer::SourceReader::fetch() {
    fchar_t c = fgetc(fp);
    if (c == '\n') {
        caret.column = 0;
        caret.line++;
    } else caret.column++;
    return c;
}

void lexer::SourceReader::resetFetchCounter(bool alreadyStarted) {
    fetchCount = alreadyStarted ? 1 : 0;
}

void lexer::SourceReader::close() {
    if (fp) fclose(fp);
    fp = nullptr;
}

