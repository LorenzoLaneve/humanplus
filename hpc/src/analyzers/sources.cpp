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

src::SrcLoc::SrcLoc(SourceFile *srcfile, long line, long column, long length) : srcfile(srcfile), line(line), column(column), length(length) {  }

src::SrcLoc::SrcLoc(const src::SrcLoc &srcloc) : SrcLoc(srcloc.srcfile, srcloc.line, srcloc.column, srcloc.length) {  }

src::SrcLoc src::SrcLoc::getNextPoint() {
    return {srcfile, line, column+length};
}

std::string src::SrcLoc::str() {
    std::ostringstream os;
    os << line+1 << ":" << column;
    return os.str();
}

void src::SrcLoc::dump(llvm::raw_ostream &stream) {
    stream << srcfile->getFileName() << ":" << line+1 << ":" << column;
}

src::SrcLoc *src::SrcLoc::join(src::SrcLoc *ref1, src::SrcLoc *ref2) {
    if (ref1->srcfile != ref2->srcfile) return nullptr;
    
    if (ref1->line < ref2->line) return new src::SrcLoc(ref1->srcfile, ref1->line, ref1->column, UntilEndOfLine);
    if (ref1->line > ref2->line) return new src::SrcLoc(ref1->srcfile, ref1->line, 1, ref1->column + ref1->length);
    
    if (ref1->column < ref2->column) return new src::SrcLoc(ref1->srcfile, ref1->line, ref1->column, ref2->column - ref1->column + ref2->length);
    if (ref1->column > ref2->column) return new src::SrcLoc(ref1->srcfile, ref1->line, ref2->column, ref1->column - ref2->column + ref1->length);
    
    return new src::SrcLoc(*ref1);
}

src::SourceFile::SourceFile(std::string filename) : fsys::InputFile(filename, fsys::SourceFile) {
    modulewrapper = new modules::ModuleWrapper(filename);
    modulewrapper->initialize();
}

modules::ModuleWrapper *src::SourceFile::getModuleWrapper() {
    return modulewrapper;
}
