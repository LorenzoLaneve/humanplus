// => src/analyzers/sources.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/sources.h>
#include <hpc/utils/printers.h>
#include <hpc/ir/modules.h>

#include <llvm/IR/LLVMContext.h>

#include <sstream>

using namespace hpc;

source::SrcLoc::SrcLoc(SourceFile *srcfile, long line, long column, long length) : srcfile(srcfile), line(line), column(column), length(length) {  }

source::SrcLoc::SrcLoc(source::SrcLoc &tkref) : srcfile(tkref.srcfile), line(tkref.line), column(tkref.column) {  }

source::SrcLoc source::SrcLoc::getNextPoint() {
    return {srcfile, line, column+length};
}

std::string source::SrcLoc::str() {
    std::ostringstream os;
    os << line+1 << ":" << column;
    return os.str();
}

void source::SrcLoc::dump(llvm::raw_ostream &stream) {
    stream << srcfile->getFileName() << ":" << line+1 << ":" << column;
}

source::SrcLoc *source::SrcLoc::join(source::SrcLoc *ref1, source::SrcLoc *ref2) {
    if (ref1->srcfile != ref2->srcfile) return nullptr;
    
    if (ref1->line < ref2->line) return new source::SrcLoc(ref1->srcfile, ref1->line, ref1->column, UntilEndOfLine);
    if (ref1->line > ref2->line) return new source::SrcLoc(ref1->srcfile, ref1->line, 1, ref1->column + ref1->length);
    
    if (ref1->column < ref2->column) return new source::SrcLoc(ref1->srcfile, ref1->line, ref1->column, ref2->column - ref1->column + ref2->length);
    if (ref1->column > ref2->column) return new source::SrcLoc(ref1->srcfile, ref1->line, ref2->column, ref1->column - ref2->column + ref1->length);
    
    return new source::SrcLoc(*ref1);
}

source::SourceFile::SourceFile(std::string filename) : fsys::InputFile(filename, fsys::SourceFile) {
    fp = fopen(filename.c_str(), "r");
    
    caret = {this};
    
    modulewrapper = new modules::ModuleWrapper(filename);
    modulewrapper->initialize();
}

source::SourceFile::~SourceFile() { this->close(); }

modules::ModuleWrapper *source::SourceFile::getModuleWrapper() {
    return modulewrapper;
}

source::SrcLoc *source::SourceFile::getCaret() {
    return new SrcLoc(this, caret.line, caret.column);
}

source::sourcechar source::SourceFile::fetchChar() {
    sourcechar c = fgetc(fp);
    if (c == '\n') {
        caret.column = 0;
        caret.line++;
    } else caret.column++;
    return c;
}

void source::SourceFile::close() {
    if (fp) fclose(fp);
    fp = nullptr;
}
