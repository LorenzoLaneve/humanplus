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

#include <hpc/analyzers/sources.h>
#include <hpc/analyzers/syntax/operators.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/runtime/runtime.h>

#include <string>

namespace hpc {
    namespace parser {
        class ParserInstance;
    }
    
    namespace lexer {
        /*!
         \brief Type describing a token returned by the lexer.
         */
        typedef int token_ty;
        
        typedef enum {
            binaryConstant = 2,
            octalConstant = 8,
            decimalConstant = 10,
            hexadecimalConstant = 16,
            exponentialConstant = -1
        } constant_type;
        
        /*!
         \brief Special tokens that may be returned by the lexer, such as compound identifiers, keywords, literals...
         */
        typedef enum {
            /*!
             \brief Token describing the end of the file the lexer read.
             */
            TokenEOF                             = -1,
            /*!
             \brief Token describing an unqualified identifier.
             */
            TokenIdentifier                      = -2,
            /*!
             \brief Token describing the \c let keyword.
             */
            TokenLet                             = -3,
            /*!
             \brief Token describing the \c namespace keyword.
             */
            TokenNameSpace                         = -4,
            /*!
             \brief Token describing the \c alias keyword.
             */
            TokenAlias                           = -5,
            /*!
             \brief Token describing the \c class keyword.
             */
            TokenClass                           = -6,
            /*!
             \brief Token describing the \c extends keyword.
             */
            TokenExtends                         = -7,
            /*!
             \brief Token describing the \c function keyword.
             */
            TokenFunction                        = -8,
            /*!
             \brief Token describing the \c as keyword.
             */
            TokenAs                              = -9,
            /*!
             \brief Token describing the \c returns keyword.
             */
            TokenReturnQualifier                 = -10,
            /*!
             \brief Token describing the \c protocol keyword.
             */
            TokenProtocol                        = -11,
            /*!
             \brief Token describing the \c null, \c nothing, \c nil keywords.
             */
            TokenNull                            = -12,
            /*!
             \brief Token describing the \c if keyword.
             */
            TokenIf                              = -13,
            /*!
             \brief Token describing the \c else keyword.
             */
            TokenElse                            = -14,
            /*!
             \brief Token describing the \c then keyword.
             */
            TokenThen                            = -15,
            /*!
             \brief Token describing the \c while keyword.
             */
            TokenWhile                           = -16,
            /*!
             \brief Token describing the \c until keyword.
             */
            TokenUntil                           = -17,
            /*!
             \brief Token describing the \c return keyword.
             */
            TokenReturnStatement                 = -18,
            /*!
             \brief Token describing the \c pointer keyword or the \c <- symbol.
             */
            TokenPointer                         = -19,
            /*!
             \brief Token describing the \c unsigned keyword.
             */
            TokenUnsigned                        = -20,
            /*!
             \brief Token describing the \c signed keyword.
             */
            TokenSigned                          = -21,
            /*!
             \brief Token describing the \c constant or \c immutable keywords.
             */
            TokenConstant                        = -22,
            /*!
             \brief Token describing the \c true or \c yes keywords.
             */
            TokenTrue                            = -23,
            /*!
             \brief Token describing the \c false or \c no keywords.
             */
            TokenFalse                           = -24,
            /*!
             \brief Token describing the \c be keyword.
             */
            TokenBe                              = -25,
            /*!
             \brief Token describing the \c nostalgic keyword.
             */
            TokenNostalgic                       = -26,
            /*!
             \brief Token describing the \c :: symbol.
             */
            TokenNameSpaceBrowser                  = -27,
            /*!
             \brief Token describing the \c implements keyword.
             */
            TokenImplements                      = -28,
            /*!
             \brief Token describing the \c do keyword.
             */
            TokenDo                              = -29,
            /*!
             \brief Token describing the \c for keyword.
             */
            TokenFor                             = -30,
            /*!
             \brief Token describing the \c and keyword.
             */
            TokenOperatorLogicalAnd              = -31,
            /*!
             \brief Token describing the \c or keyword.
             */
            TokenOperatorLogicalOr               = -32,
            /*!
             \brief Token describing the \c switch keyword.
             */
            TokenSwitch                          = -32,
            /*!
             \brief Token describing the \c break keyword.
             */
            TokenBreak                           = -33,
            /*!
             \brief Token describing the \c continue keyword.
             */
            TokenContinue                        = -34,
            
            /*!
             \brief Token describing a number literal that fits into an integer.
             */
            TokenIntegerLiteral                  = -50,
            /*!
             \brief Token describing a number literal that fits into an unsigned integer.
             */
            TokenUnsignedIntegerLiteral          = -51,
            /*!
             \brief Token describing a number literal that fits into a long.
             */
            TokenLongLiteral                     = -52,
            /*!
             \brief Token describing a number literal that fits into an unsigned long.
             */
            TokenUnsignedLongLiteral             = -53,
            /*!
             \brief Token describing a number literal that fits into a float.
             */
            TokenFloatLiteral                    = -54,
            /*!
             \brief Token describing a number literal that fits into a double.
             */
            TokenDoubleLiteral                   = -55,
            /*!
             \brief Token describing an ASCII character literal.
             */
            TokenCharacterLiteral                = -56,
            /*!
             \brief Token describing an ASCII string literal.
             */
            TokenStringLiteral                   = -57,
            
            /*!
             \brief Token describing the \c void keyword.
             */
            TokenTypeVoid                        = -70,
            /*!
             \brief Token describing the \c bool or \c boolean keywords.
             */
            TokenTypeBool                        = -71,
            /*!
             \brief Token describing the \c char or \c character keywords.
             */
            TokenTypeChar                        = -72,
            /*!
             \brief Token describing the \c byte keyword.
             */
            TokenTypeByte                        = -73,
            /*!
             \brief Token describing the \c short keyword.
             */
            TokenTypeShort                       = -74,
            /*!
             \brief Token describing the \c int or \c integer keywords.
             */
            TokenTypeInteger                     = -75,
            /*!
             \brief Token describing the \c long keyword.
             */
            TokenTypeLong                        = -76,
            /*!
             \brief Token describing the \c float or \c single keywords.
             */
            TokenTypeFloat                       = -77,
            /*!
             \brief Token describing the \c double keyword.
             */
            TokenTypeDouble                      = -78,
            
            /*!
             \brief Token describing the \c + symbol.
             */
            TokenOperatorPlus                    = '+',
            /*!
             \brief Token describing the \c - symbol.
             */
            TokenOperatorMinus                   = '-',
            /*!
             \brief Token describing the \c * symbol.
             */
            TokenOperatorMultiply                = '*',
            /*!
             \brief Token describing the \c / symbol.
             */
            TokenOperatorDivide                  = '/',
            /*!
             \brief Token describing the \c % symbol.
             */
            TokenOperatorRemainder               = '%',
            
            /*!
             \brief Token describing the \c < symbol.
             */
            TokenOperatorLower                   = '<',
            /*!
             \brief Token describing the \c > symbol.
             */
            TokenOperatorGreater                 = '>',
            /*!
             \brief Token describing the \c <= symbol.
             */
            TokenOperatorLowerEqual              = -100,
            /*!
             \brief Token describing the \c >= symbol.
             */
            TokenOperatorGreaterEqual            = -101,
            /*!
             \brief Token describing the \c == symbol.
             */
            TokenOperatorEqual                   = -102,
            /*!
             \brief Token describing the \c != symbol.
             */
            TokenOperatorNotEqual                = -103,
            /*!
             \brief Token describing the \c << symbol.
             */
            TokenOperatorLeftShift               = -104,
            /*!
             \brief Token describing the \c >> symbol.
             */
            TokenOperatorRightShift              = -105,
            /*!
             \brief Token describing the \c & symbol.
             */
            TokenOperatorBitwiseAnd              = '&',
            /*!
             \brief Token describing the \c | symbol.
             */
            TokenOperatorBitwiseOr               = '|',
            
            /*!
             \brief Token describing the \c . symbol.
             */
            TokenOperatorMemberAccess            = -110,
            
            /*!
             \brief Token describing the \c = symbol.
             */
            TokenOperatorAssign                  = '=',
            /*!
             \brief Token describing the \c += symbol.
             */
            TokenOperatorPlusEqual               = __operator_attach_assignment(TokenOperatorPlus),
            /*!
             \brief Token describing the \c -= symbol.
             */
            TokenOperatorMinusEqual              = __operator_attach_assignment(TokenOperatorMinus),
            /*!
             \brief Token describing the \c *= symbol.
             */
            TokenOperatorMultiplyEqual           = __operator_attach_assignment(TokenOperatorMultiply),
            /*!
             \brief Token describing the \c /= symbol.
             */
            TokenOperatorDivideEqual             = __operator_attach_assignment(TokenOperatorDivide),
            /*!
             \brief Token describing the \c %= symbol.
             */
            TokenOperatorRemainderEqual          = __operator_attach_assignment(TokenOperatorRemainder),
            
            /*!
             \brief Token describing the \c not keyword or the \c ! symbol.
             */
            TokenOperatorExclMark                = '!',
            /*!
             \brief Token describing the \c ~ symbol.
             */
            TokenOperatorTilde                   = '~',
        } SpecialToken;
        
        /*!
         \brief Object which reads and tokenizes the input files.
         */
        class LexerInstance {
            friend class parser::ParserInstance;
            
            /*!
             \brief The diagnostics engine the lexer has to report diagnostics to.
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
             \brief Closes the stream to the source.
             */
            void close();
            
            
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
             \brief Sets the token to the next literal in the source stream.
             */
            void checkLiteral(Token &newToken);
            
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
        
        /*!
         \brief Returns whether \c c is a valid delimiter for Human Plus statements and declarations.
         */
        bool isDelimiter(char c);
        
    }
}

#endif
