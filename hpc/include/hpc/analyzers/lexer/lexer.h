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
             \brief The source file the lexer is reading from.
             \note This file should be closed by calling \c unbind() on the \c LexerInstance object.
             */
            source::SourceFile *sourcefile;
            
            /*!
             \brief The last char read from the lexer stream.
             */
            source::sourcechar lastChar;
            
            /*!
             \brief The reference in the source for the current token.
             */
            source::TokenRef currentRef;
            /*!
             \brief The current token.
             */
            lexer::token_ty currentToken;
            
            /*!
             \brief The reference in the source for the token read before the current token.
             */
            source::TokenRef lastRef;
            /*!
             \brief The token read before the current token, or \c 0 if the second token in the stream has not been read yet.
             */
            lexer::token_ty lastToken = 0;
            
            /*!
             \brief The last identifier read by the lexer.
             \note Not only unqualified identifiers are saved here.
             */
            std::string currentIdentifier;
            
            /*!
             \brief The last \c character literal read by the lexer.
             \note The value is undefined until the first literal is encountered.
             */
            runtime::utf7_char_ty currentCharacter;
            /*!
             \brief The last \c integer literal read by the lexer.
             \note The value is undefined until the first literal is encountered.
             */
            runtime::int32_ty currentInteger;
            /*!
             \brief The last <code>unsigned integer</code> literal read by the lexer.
             \note The value is undefined until the first literal is encountered.
             */
            runtime::uint32_ty currentUnsignedInteger;
            /*!
             \brief The last \c long literal read by the lexer.
             \note The value is undefined until the first literal is encountered.
             */
            runtime::int64_ty currentLong;
            /*!
             \brief The last <code>unsigned long</code> literal read by the lexer.
             \note The value is undefined until the first literal is encountered.
             */
            runtime::uint64_ty currentUnsignedLong;
            /*!
             \brief The last \c float literal read by the lexer.
             \note The value is undefined until the first literal is encountered.
             */
            runtime::fp_single_ty currentFloat;
            /*!
             \brief The last \c double literal read by the lexer.
             \note The value is undefined until the first literal is encountered.
             */
            runtime::fp_double_ty currentDouble;
            /*!
             \brief The last \c string literal read by the lexer.
             \note The value is undefined until the first literal is encountered.
             */
            runtime::string_ty currentString;
            
            /*!
             \brief Number used to keep the count of how many characters has been read to compose a token.
             */
            unsigned long fetchCount = 0;
            
            /*!
             \brief Returns the next token from the stream.
             \note This method <b>does not</b> update the lexer instance with the new token. It is used by \c getNextToken()
             */
            token_ty getNewToken(source::TokenRef *tkref = nullptr);
            
            /*!
             \brief Finalizes the lexer and unbinds the current source file.
             */
            void unbind();
            /*!
             \brief Returns a pointer to the \c SourceFile object describing the source file the lexer is currently reading.
             */
            source::SourceFile *getSourceFile();
            
            /*!
             \brief Reads the next character from the bound source file stream. Assertion will occur if no source file was bound to the lexer.
             */
            source::sourcechar fetch();
            /*!
             \brief Returns the number of characters fetched from the bound source file by the lexer since the beginning or since \c lexer::resetFetchCount() was called.
             */
            unsigned long getFetchCount();
            /*!
             \brief Resets the number of fetches made by the lexer on the bound source file.
             \param alreadyStarted A boolean saying whether the lexer should assume the current character should be included in the count.
             */
            void resetFetchCount(bool alreadyStarted = true);
            
            
            /*!
             \brief Returns whether the bound source is finished.
             \return \c true if the lexer has found \c EOF in the bound source file, \c false otherwise.
             \note You <u>should not</u> make any further call to \c lexer::getNextToken() after this function started returning true.
             */
            bool eof();
            /*!
             \brief Ignores all the tokens and jumps to the first token after a delimiter or after a scope \c {}.
             \return \c false if \c EOF was found while escaping, \c true otherwise.
             */
            bool escape();
            
            /*!
             \brief Reads a new token from the bound source file and returns it. The new token will be saved in the \c currentToken, and the old \c currentToken will be saved in the \c lastToken.
             \note This function <b>must</b> take \c lastChar as first character and <b>must</b> read a character it doesn't use and putting it in \c lastChar before returning. The unused character read will be the first character for the next call to this function.
             \param tkref Pointer to a \c source::TokenRef struct where the lexer will save information about the location of the token in the source file.
             \return A value in the \c Token enum describing the token the lexer recognized, or the first non-space character if no known token could be read.
             */
            token_ty getNextToken(source::TokenRef *tkref = nullptr);
            /*!
             \brief Returns the last token the lexer has read in the bound source file.
             \param tkref Pointer to a \c source::TokenRef struct where the lexer will save information about the location of the token in the source file.
             */
            token_ty getCurrentToken(source::TokenRef *tkref = nullptr);
            /*!
             \brief Returns the token before the last token the lexer has read in the bound source file.
             \param tkref Pointer to a \c source::TokenRef struct where the lexer will save information about the location of the token in the source file.
             */
            token_ty getLastToken(source::TokenRef *tkref = nullptr);
            /*!
             \brief Returns a \c std::string containing the identifier corresponding to the current token.
             \warning If \c lexer::getCurrentToken() \c != \c lexer::TokenIdentifier the string returned by this function is undefined.
             */
            std::string getCurrentIdentifier();
            /*!
             \brief Reads the character as character literal, parsing any escape sequences found.
             \param quote Character containing the quote that should end the literal.
             */
            runtime::utf7_char_ty getASCIIChar(source::sourcechar quote);
            /*!
             \brief Passes the characters in the file until the C-style multiline comment is closed.
             \return \c false if EOF was found before the comment is closed, \c true otherwise.
             */
            bool ignoreMultilineComment();
            /*!
             \brief Resets the tokenizer, initializing current and last tokens to null tokens.
             */
            void resetTokenizer();
            
            /*!
             \brief Passes the current token if this is an english article ('a' or 'an').
             */
            void ignoreArticleIfAny();
            /*!
             \brief Passes the current token if this is an english relative pronoun ('that' or 'which').
             */
            void ignoreRelativePronounsIfAny();
            /*!
             \brief Ignores the current token if this is an 'and'.
             */
            void ignoreAndIfAny();
            

            /*!
             \brief Puts the given number in the lexer's constant set, under the right type.
             */
            int putDecimalConstant(std::string numberString, lexer::token_ty suffixtype, source::TokenRef *literalRef);
            /*!
             \brief Puts the given hexadecimal number in the lexer's constant set, under the right type.
             */
            int putHexadecimalConstant(std::string numberString, lexer::token_ty suffixtype, source::TokenRef *literalRef);
            /*!
             \brief Puts the given binary number in the lexer's constant set, under the right type.
             */
            int putBinaryConstant(std::string numberString, lexer::token_ty suffixtype, source::TokenRef *literalRef);
            /*!
             \brief Puts the given octal hexadecimal number in the lexer's constant set, under the right type.
             */
            int putOctalConstant(std::string numberString, lexer::token_ty suffixtype, source::TokenRef *literalRef);

        public:
            virtual ~LexerInstance();
            /*!
             \brief Initializes the lexer instance with the compilation driver and the pathname of the file being read.
             */
            LexerInstance(diag::DiagEngine &diags, source::SourceFile *sourceFile);
            
        };
        
        /*!
         \brief Returns whether \c c is a valid delimiter for Human Plus statements and declarations.
         */
        bool isDelimiter(char c);
        
    }
}

#endif
