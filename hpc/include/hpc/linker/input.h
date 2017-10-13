// => hpc/linker/input.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_linker_input
#define __human_plus_compiler_linker_input

#include <llvm/Support/ToolOutputFile.h>

#include <string>
#include <vector>

namespace hpc {
    namespace linker {

        /*!
         \brief Returns a \c std::vector containing the paths for the object files that are passed to the linker when \c linker::callSystemLinker() is called.
         */
        std::vector<std::string> &getInputObjectFiles();
        /*!
         \brief Adds the given object file to the list of files to be passed to the linker.
         \param objfile A string containing the path for the object file.
         \param filepointer A \c llvm::tool_output_file object holding the stream for the created object file, or \c nullptr if the object file has not been created by the compiler.
         */
        void pushObjectFile(std::string objfile, llvm::tool_output_file *filepointer = nullptr);
    }
}

#endif
