// => hpc/analyzers/lexer/tokens.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_lexer_tokens
#define __human_plus_compiler_lexer_tokens

#include <hpc/analyzers/sources.h>
#include <hpc/analyzers/syntax/literals.h>
#include <hpc/analyzers/syntax/operators.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/runtime/runtime.h>

#include <stdio.h> // for EOF
#include <string>
#include <assert.h>





namespace hpc {
    
    namespace lexer {
        
        class LexerInstance;
        
        /*!
         \brief Class holding the information about a token read by the lexer.
         */
        class Token {
            friend class LexerInstance;

        public:
            typedef unsigned int Type;
            /*!
             \brief Values for special token types.
             */
            typedef enum {
                /*!
                 \brief Token describing the end of the file.
                 */
                EndOfFile = EOF,
                /*!
                 \brief Token describing an unqualified identifier.
                 */
                Identifier = -2,
                /*!
                 \brief Token describing the \c let keyword.
                 */
                Let = -3,
                /*!
                 \brief Token describing the \c namespace keyword.
                 */
                NameSpace = -4,
                /*!
                 \brief Token describing the \c alias keyword.
                 */
                Alias = -5,
                /*!
                 \brief Token describing the \c class keyword.
                 */
                Class = -6,
                /*!
                 \brief Token describing the \c extends keyword.
                 */
                Extends = -7,
                /*!
                 \brief Token describing the \c function keyword.
                 */
                Function = -8,
                /*!
                 \brief Token describing the \c as keyword.
                 */
                As = -9,
                /*!
                 \brief Token describing the \c returns keyword.
                 */
                Returns = -10,
                /*!
                 \brief Token describing the \c protocol keyword.
                 */
                Protocol = -11,
                /*!
                 \brief Token describing the \c null, \c nothing, \c nil keywords.
                 */
                Null = -12,
                /*!
                 \brief Token describing the \c if keyword.
                 */
                If = -13,
                /*!
                 \brief Token describing the \c else keyword.
                 */
                Else = -14,
                /*!
                 \brief Token describing the \c then keyword.
                 */
                Then = -15,
                /*!
                 \brief Token describing the \c while keyword.
                 */
                While = -16,
                /*!
                 \brief Token describing the \c until keyword.
                 */
                Until = -17,
                /*!
                 \brief Token describing the \c return keyword.
                 */
                Return = -18,
                /*!
                 \brief Token describing the \c pointer keyword or the \c <- symbol.
                 */
                Pointer = -19,
                /*!
                 \brief Token describing the \c unsigned keyword.
                 */
                Unsigned = -20,
                /*!
                 \brief Token describing the \c signed keyword.
                 */
                Signed = -21,
                /*!
                 \brief Token describing the \c constant or \c immutable keywords.
                 */
                Constant = -22,
                /*!
                 \brief Token describing the \c true or \c yes keywords.
                 */
                True = -23,
                /*!
                 \brief Token describing the \c false or \c no keywords.
                 */
                False = -24,
                /*!
                 \brief Token describing the \c be keyword.
                 */
                Be = -25,
                /*!
                 \brief Token describing the \c nostalgic keyword.
                 */
                Nostalgic = -26,
                /*!
                 \brief Token describing the \c :: symbol.
                 */
                NameSpaceBrowser = -27,
                /*!
                 \brief Token describing the \c implements keyword.
                 */
                Implements = -28,
                /*!
                 \brief Token describing the \c do keyword.
                 */
                Do = -29,
                /*!
                 \brief Token describing the \c for keyword.
                 */
                For = -30,
                /*!
                 \brief Token describing the \c and keyword.
                 */
                And = -31,
                /*!
                 \brief Token describing the \c or keyword.
                 */
                Or = -32,
                /*!
                 \brief Token describing the \c switch keyword.
                 */
                Switch = -32,
                /*!
                 \brief Token describing the \c break keyword.
                 */
                Break = -33,
                /*!
                 \brief Token describing the \c continue keyword.
                 */
                Continue = -34,
                
                /*!
                 \brief Token describing a number literal.
                 */
                NumberLiteral = -50,
                /*!
                 \brief Token describing an ASCII character literal.
                 */
                CharLiteral = -56,
                /*!
                 \brief Token describing an ASCII string literal.
                 */
                StringLiteral = -57,
                
                /*!
                 \brief Token describing the \c void keyword.
                 */
                TypeVoid = -70,
                /*!
                 \brief Token describing the \c bool or \c boolean keywords.
                 */
                TypeBool = -71,
                /*!
                 \brief Token describing the \c char or \c character keywords.
                 */
                TypeChar = -72,
                /*!
                 \brief Token describing the \c byte keyword.
                 */
                TypeByte = -73,
                /*!
                 \brief Token describing the \c short keyword.
                 */
                TypeShort = -74,
                /*!
                 \brief Token describing the \c int or \c integer keywords.
                 */
                TypeInteger = -75,
                /*!
                 \brief Token describing the \c long keyword.
                 */
                TypeLong = -76,
                /*!
                 \brief Token describing the \c float or \c single keywords.
                 */
                TypeFloat = -77,
                /*!
                 \brief Token describing the \c double keyword.
                 */
                TypeDouble = -78,
                
                /*!
                 \brief Token describing the \c . symbol.
                 */
                MemberAccess = '.',
                
                
#define __bin_operator_def(id, toString, tokenValue, precedence, associativity) id = tokenValue,
#define __un_operator_def(id, toString, tokenValue) id = tokenValue,
                
#include <hpc/inc/syntax/operators.inc>
                
#undef __bin_operator_def
#undef __un_operator_def
                
            } SpecialType;
            
        private:
            /*!
             \brief Type of the token.
             */
            Type type;
            /*!
             \brief The location of this token in the sources.
             */
            src::SrcLoc *location = nullptr;
            /*!
             \brief The identifier associated with the token, if any.
             */
            std::string *identifier = nullptr;
            /*!
             \brief The parsed number literal associated with the token, if any.
             */
            syntax::NumberLiteral *numberLiteral = nullptr;
            /*!
             \brief The parsed character literal associated with the token, if any.
             */
            syntax::CharacterLiteral *charLiteral = nullptr;
            /*!
             \brief The parsed string literal associated with the token, if any.
             */
            syntax::StringLiteral *stringLiteral = nullptr;
            
        public:
            Token() {  }
            
            ~Token();
            
            /*!
             \brief Returns the type of the token.
             */
            inline Type getTokenType() const {
                return type;
            }
            /*!
             \brief Returns a \c SrcLoc object containing information about the token location in the source.
             \warning This object will be freed with the Token object and should be copied if stored elsewhere.
             */
            inline src::SrcLoc *getSrcLoc() const {
                return location;
            }
            /*!
             \brief The identifier associated with the token, if the token is an identifier token.
             */
            inline const std::string &getIdentifier() const {
                assert(identifier && "The token is not an identifier.");
                return *identifier;
            }
            /*!
             \brief The number literal associated with the token, if the token is a number literal.
             */
            inline const syntax::NumberLiteral &getNumberLiteral() const {
                assert(numberLiteral && "The token is not an integer literal.");
                return *numberLiteral;
            }
            /*!
             \brief The character literal associated with the token, if the token is a character literal.
             */
            inline const syntax::CharacterLiteral &getCharacterLiteral() const {
                assert(charLiteral && "The token is not a character literal.");
                return *charLiteral;
            }
            /*!
             \brief The string literal associated with the token, if the token is a string literal.
             */
            inline const syntax::StringLiteral &getStringLiteral() const {
                assert(stringLiteral && "The token is not a string literal.");
                return *stringLiteral;
            }

            /*!
             \brief Returns whether the token is of the given type.
             */
            inline bool isType(const Type tokenType) const {
                return type == tokenType;
            }
            
            inline bool operator==(const Type tokenType) const {
                return isType(tokenType);
            }
            inline bool operator!=(const Type tokenType) const {
                return !isType(tokenType);
            }
        };
        
    }
}

#endif
