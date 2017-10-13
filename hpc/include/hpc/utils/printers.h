// => hpc/utils/printers.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_printers
#define __human_plus_compiler_printers

#include <hpc/config.h>

#include <llvm/Support/raw_ostream.h>

#include <string>


#ifdef __hpc_debugging
/*!
 \brief Prints an array of elements to the standard output, only if hpc is compiled in debug mode.
 \note To join more than one element you can use the \c << operator.
 */
#define dblog(x) llvm::outs() << x
#else
#define dblog(x)
#endif

namespace hpc {
    namespace util {
        
        /*!
         \brief Prints the ASCII character to standard output. The character will be converted into a C-like escaped sequence corresponding to the character if any.
         */
        void printEscapedChar(char c); // TO REMOVE
        
        /*!
         \brief Prints the ASCII string to standard output. The string will be converted into a C-like string literal (without quotes).
         */
        void printEscapedString(std::string s); // TO REMOVE
        
    }
}

#endif
