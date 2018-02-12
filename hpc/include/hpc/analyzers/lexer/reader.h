// => hpc/analyzers/lexer/reader.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_lexer_reader
#define __human_plus_compiler_lexer_reader

#include <hpc/analyzers/sources.h>

#include <stdio.h>

namespace hpc {
    
    namespace lexer {
        
        typedef unsigned int fchar_t;
        
        /*!
         \brief Class which provides utilities for reading a source file.
         */
        class SourceReader {
            /*!
             \brief The object for the source file to be read.
             */
            src::SourceFile &source;
            /*!
             \brief The C file pointer pointing to the file being read.
             */
            FILE *fp;
            /*!
             \brief A pointer to the buffer.
             */
            unsigned char *buffer;
            /*!
             \brief Pointer to the next element to be read from the buffer.
             */
            unsigned char *nextbuf;
            /*!
             \brief Characters in the buffer that still have to be read.
             */
            int leftChars;
            /*!
             \brief Counter for the number of fetched characters.
             */
            int fetchCount;
            
            /*!
             \brief A \c SrcLoc structure pointing to the last character the lexer read from this file.
             */
            src::SrcLoc caret = {0, 0};
            
            /*!
             \brief Fills up the buffer by reading the characters in the file.
             */
            void fillUpBuffer();
            /*!
             \brief Returns the next character in the buffer stream, according to the encoding.
             */
            fchar_t readChar();
            
        public:
            /*!
             \brief Initializes the reader with the object for the source file to be read.
             */
            SourceReader(src::SourceFile &source);
            
            SourceReader(SourceReader &) = delete;
            SourceReader &operator=(SourceReader &) = delete;
            
            ~SourceReader();
            
            /*!
             \brief Returns whether the stream is ready to be read.
             */
            inline bool good() const {
                return fp != nullptr;
            }
            /*!
             \brief Returns whether the end of file is reached.
             */
            inline bool eof() const {
                return lastChar() == EOF;
            }
            /*!
             \brief Returns the last character read in the buffer stream.
             */
            inline fchar_t lastChar() const {
                return *nextbuf;
            }
            /*!
             \brief Reads a new character from the stream and updates the caret.
             */
            fchar_t fetch();
            
            /*!
             \brief Sets the fetch counter to 0.
             \param alreadyStarted If set to \c true, the last read character will be counted.
             */
            void resetFetchCounter(bool alreadyStarted = false);
            /*!
             \brief Returns the number of character fetched since the last reset.
             */
            inline int getFetchCount() const {
                return fetchCount;
            }
            /*!
             \brief Returns a copy of the SrcLoc object describing the location of the last read character in the source.
             */
            inline src::SrcLoc *getCaret() const {
                return new src::SrcLoc(caret);
            }
            
            /*!
             \brief Closes the stream to the associated file.
             */
            void close();

        };
        
    }
}

#endif
