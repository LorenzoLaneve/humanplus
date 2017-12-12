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
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/runtime/runtime.h>

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

            typedef unsigned int Type;
            /*!
             \brief Values for special token types.
             */
            typedef enum {
                
            } SpecialType;
            
            /*!
             \brief Type of the token.
             */
            Type type;
            /*!
             \brief The location of this token in the sources.
             */
            source::SrcLoc *location = nullptr;
            /*!
             \brief The identifier associated with the token, if any.
             */
            std::string *identifier = nullptr;
            /*!
             \brief The parsed integer literal associated with the token, if any.
             */
            rt::int32_ty *intLiteral = nullptr;
            /*!
             \brief The parsed unsigned integer literal associated with the token, if any.
             */
            rt::uint32_ty *uintLiteral = nullptr;
            /*!
             \brief The parsed long literal associated with the token, if any.
             */
            rt::int64_ty *longLiteral = nullptr;
            /*!
             \brief The parsed unsigned long literal associated with the token, if any.
             */
            rt::uint64_ty *ulongLiteral = nullptr;
            /*!
             \brief The single floating point literal associated with the token, if any.
             */
            rt::fp_single_ty *floatLiteral = nullptr;
            /*!
             \brief The double floating point literal associated with the token, if any.
             */
            rt::fp_double_ty *doubleLiteral = nullptr;
            
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
            inline source::SrcLoc *getSrcLoc() const {
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
             \brief The value of the integer literal associated with the token, if the token is an integer literal.
             */
            inline rt::int32_ty getIntLiteral() const {
                assert(intLiteral && "The token is not an integer literal.");
                return *intLiteral;
            }
            /*!
             \brief The value of the unsigned integer literal associated with the token, if the token is an unsigned integer literal.
             */
            inline rt::uint32_ty getUIntLiteral() const {
                assert(uintLiteral && "The token is not an unsigned integer literal.");
                return *uintLiteral;
            }
            /*!
             \brief The value of the long literal associated with the token, if the token is a long literal.
             */
            inline rt::int64_ty getLongLiteral() const {
                assert(longLiteral && "The token is not a long literal.");
                return *longLiteral;
            }
            /*!
             \brief The value of the unsigned long literal associated with the token, if the token is an unsigned long literal.
             */
            inline rt::uint64_ty getULongLiteral() const {
                assert(ulongLiteral && "The token is not an unsigned long literal.");
                return *ulongLiteral;
            }
            /*!
             \brief The value of the float literal associated with the token, if the token is a float literal.
             */
            inline rt::fp_single_ty getFloatLiteral() const {
                assert(floatLiteral && "The token is not a floating point literal.");
                return *floatLiteral;
            }
            /*!
             \brief The value of the double literal associated with the token, if the token is a double literal.
             */
            inline rt::fp_double_ty getDoubleLiteral() const {
                assert(doubleLiteral && "The token is not a double floating point literal.");
                return *doubleLiteral;
            }

            /*!
             \brief Returns whether the token is of the given type.
             */
            inline bool isType(Type tokenType) const {
                return type == tokenType;
            }
            
            inline bool operator==(Type tokenType) const {
                return isType(tokenType);
            }
            inline bool operator!=(Type tokenType) const {
                return !isType(tokenType);
            }
        };
        
    }
}

#endif
