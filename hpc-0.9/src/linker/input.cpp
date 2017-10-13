//
//  input.cpp
//  HumanPlus
//
//  Created by Lore on 31/03/16.
//  Copyright © 2016 Lore. All rights reserved.
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