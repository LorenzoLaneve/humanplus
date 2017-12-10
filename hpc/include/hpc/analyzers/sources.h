// => hpc/analyzers/sources.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_sources
#define __human_plus_compiler_sources

#include <hpc/utils/files.h>

#include <llvm/Support/raw_ostream.h>

#include <cstdio>
#include <string>
#include <vector>

namespace hpc {
    
    namespace modules {
        class ModuleWrapper;
    }
    
    namespace source {
        
        /*!
         \brief Type for characters read from source files.
         */
        typedef int sourcechar;
        
        class SourceFile;
        
        /*!
         \brief A structure pointing to a little part of code in a specific source file.
         */
        struct SrcLoc {
            enum {
                /*!
                 \brief Value that should be give as \c length for \c SrcLoc 's who should continue until the end of the line.
                 */
                UntilEndOfLine = -1
            };
            
            /*!
             \brief A pointer to the \c SourceFile object describing the file where the pointed chunk of code can be found.
             */
            SourceFile *srcfile;
            
            /*!
             \brief The number of the line in the source file where the pointed chunk of code can be found.
             \note In the structure, the line will start from \c 0, while it will start from \c 1 when displayed to the user.
             */
            long line;
            /*!
             \brief The number of characters between the start of the pointing chunk of code and the begin of the \c line, start included.
             */
            long column;
            /*!
             \brief The length of the pointing chunk of code.
             \note The length is usually not displayed to the user. It usually comes up when the output has to show the chunk of code, or when an interfacing IDE needs the length of the chunk of code it should underline/highlight.
             */
            long length;
            
            /*!
             \brief Constructor for \c SrcLoc that simply initializes all the members.
             */
            SrcLoc(SourceFile *srcfile = nullptr, long line = 0, long column = 0, long length = 0);
            /*!
             \brief Copy constructor.
             */
            SrcLoc(SrcLoc &tkref);
            
            /*!
             \brief Returns a new \c SrcLoc structure pointing to the character immediately next to the chunk of code pointed by this structure.
             \code
             he's pointing me...
             ^~~~~~~~~~~~~~~~~~~ this
             
             he's pointing me...
                                ^ this->getNextPoint()
             \endcode
             */
            SrcLoc getNextPoint();
            
            /*!
             \brief Returns a \c std::string with the format \c <line>:<column>
             */
            std::string str();
            
            /*!
             \brief Prints the object to the given output with the format \c <file>:<line>:<column>
             */
            void dump(llvm::raw_ostream &stream = llvm::outs());
            
            /*!
             \brief Returns a pointer to a \c SrcLoc structure that points to the whole chunk of code included by the two \c SrcLoc 's
             \note If \c ref1 and \c ref2 point to different files, \c nullptr is returned.
             \code
             abc + xyz        abc + xyz
             ^~~   ^~~   =>   ^~~~~~~~~
             \endcode
             */
            static SrcLoc *join(SrcLoc *ref1, SrcLoc *ref2);
        };
        
        /*!
         \brief An object indicating a source file given in input to the compiler.
         */
        class SourceFile : public fsys::InputFile {
            /*!
             \brief A C file pointer to the associated file.
             \note This pointer is set to \c NULL when \c close() is called.
             */
            FILE *fp;
            /*!
             \brief The wrapper for the LLVM module associated to this source file. All the IR generation for this source file will be handled by this module wrapper.
             */
            modules::ModuleWrapper *modulewrapper;
            /*!
             \brief A \c SrcLoc structure pointing to the last character the lexer read from this file.
             */
            SrcLoc caret = {0, 0};
            
        public:
            /*!
             \brief Initializes the object with the pathname contained in \c filename and opens the stream to the file.
             */
            SourceFile(std::string filename);
            /*!
             \brief When the object is destroyed, the associated file stream will be closed.
             */
            virtual ~SourceFile();
            
            
            inline bool isOk() const {
                return fp;
            }
            
            /*!
             \brief Returns the LLVM module wrapper associated to this source file. All the IR generation for this file will be handled by this module wrapper.
             */
            modules::ModuleWrapper *getModuleWrapper();
            /*!
             \brief Returns a \c SrcLoc structure pointing to the last character the lexer read from this file.
             */
            SrcLoc *getCaret();
            
            /*!
             \brief Reads the next character from the associated file, and updates the caret.
             */
            sourcechar fetchChar();
            
            /*!
             \brief Closes the stream with the associated file.
             */
            void close();
        };
        
    }
}

#endif
