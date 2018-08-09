// => hpc/analyzers/lexer/lexer.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_lexer
#define __human_plus_compiler_lexer

#include <hpc/analyzers/lexer/tokens.h>
#include <hpc/analyzers/lexer/reader.h>
#include <hpc/analyzers/syntax/literals.h>
#include <hpc/analyzers/sources.h>
#include <hpc/diagnostics/diagnostics.h>

#include <string>

namespace hpc {
    namespace lexer {
        
        /*!
         \brief Values for lexer contexts. The lexer will recognize different keywords based on the context, making the language more flexible.
         */
        typedef enum {
            ContextMain
        } LexerContext;
        
        class LexerInstance {
            /*!
             \brief The diagnostics engine used to report any lexer errors.
             */
            diag::DiagEngine &diags;
            /*!
             \brief The object for the source file being read.
             */
            src::SourceFile &source;
            /*!
             \brief The current lexing context of the lexer.
             */
            LexerContext currentContext;
            

            /*!
             \brief The token read before the current token.
             */
            Token *lastToken;
            /*!
             \brief The current token, the last token read with \c getNextToken().
             */
            Token *currentToken;
            
            
        public:
            /*!
             \brief Initializes the lexer instance with the diagnostics engine that will be used to report any errors, and the object for the source file that will be open for reading.
             */
            LexerInstance(diag::DiagEngine &diags, src::SourceFile &source);
            
            ~LexerInstance();
            
            /*!
             \brief Returns the diagnostics engine used by the lexer.
             */
            inline diag::DiagEngine &getDiags() const {
                return diags;
            }
            /*!
             \brief Returns the object for the source file being read.
             */
            inline src::SourceFile &getSourceFile() const {
                return source;
            }
            /*!
             \brief Returns the lexer's current context.
             */
            inline LexerContext getCurrentContext() const {
                return currentContext;
            }
            
            
            /// READER
        private:
            /*!
             \brief The reader which keeps the source file stream.
             */
            SourceReader *reader = nullptr;
            
            /*!
             \brief Returns the associated source reader.
             */
            inline SourceReader &getSourceReader() const {
                assert(reader && "No reader opened.");
                return *reader;
            }
            /*!
             \brief Initializes and opens a new source reader.
             */
            void open();
            /*!
             \brief Reads the next character in the associated source file stream.
             */
            inline fchar_t fetch() {
                return getSourceReader().fetch();
            }
            /*!
             \brief Returns the source location for the last read character.
             */
            inline src::SrcLoc *getCaret() {
                return getSourceReader().getCaret();
            }
            /*!
             \brief Closes the stream to the source.
             */
            void close();
            
        public:
            /*!
             \brief Returns whether the lexer source is at the end of file.
             */
            inline bool eof() const {
                return currentToken->type == Token::EndOfFile;
            }
            /*!
             \brief Tries to eat character until a delimiter or a closing brace is found.
             \return \c false iff an \c EOF is found.
             */
            bool escape();
            
            
            /// TOKENIZER
        private:
            /*!
             \brief The last character read from the source. Value used by the tokenizer.
             */
            fchar_t lastChar;
            
            /*!
             \brief Reads a new token from the source and sets its information in the object passed by reference.
             */
            void readNewToken(Token &newToken);
            
            /*!
             \brief Sets the token id to the keyword id matching the given identifier, according to the current lexer context.
             \return \c true if a matching keyword has been found, \c false if this identifier is unqualified.
             */
            bool checkKeyword(Token &newToken, std::string identifier);
            
            /*!
             \brief Sets the token to the next number literal in the source stream, starting from the first digit/dot.
             \return This method will return \c false iff the first character is a dot and the following is not a valid digit. In this case, the lexer will assume the token is not a number literal, and then a \c MemberAccess token should be set.
             */
            bool checkNumberLiteral(Token &newToken);
            
            /*!
             \brief Tries to read in the source stream an escape sequence to form a character literal.
             \return \true if the sequence has been recognized, \c false otherwise.
             \note An error will be sent to the lexer's diagnostics engine by this method in case of unknown sequences.
             */
            bool checkCharEscapeSequence(syntax::CharData &cdata);
            /*!
             \brief Sets the token to the next character literal in the source stream.
             */
            void checkCharLiteral(Token &newToken);
            /*!
             \brief Sets the token to the next string literal in the source stream.
             */
            void checkStringLiteral(Token &newToken);

            /*!
             \brief Reads any character until a \n is found.
             \return \c false iff \c EOF is found.
             */
            bool ignoreInlineComment();
            /*!
             \brief Reads any character until a * followed by a / is found. Nested comments are recursively handled.
             \return \c false iff \c EOF is found.
             */
            bool ignoreMultilineComment();
            
        public:
            
            /*!
             \brief Returns the token before the current token.
             */
            Token &getLastToken();
            /*!
             \brief Returns the current token, the last token being read.
             */
            Token &getCurrentToken();
            /*!
             \brief Reads a new token from the lexer, and puts it as current token, returning it.
             */
            Token &getNextToken();
            
        };
        
    }
}

#endif
