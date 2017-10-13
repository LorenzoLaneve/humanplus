// => src/linker/input.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/linker/input.h>

using namespace hpc;

static std::vector<std::string> objectfiles;
static std::vector<llvm::tool_output_file *> filepointers;

std::vector<std::string> &linker::getInputObjectFiles() {
    return objectfiles;
}

void linker::pushObjectFile(std::string objfile, llvm::tool_output_file *filepointer) {
    objectfiles.push_back(objfile);
    
    if (filepointer) filepointers.push_back(filepointer);
}
