// => hpc/analyzers/lexer/literals.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_lexer_literals
#define __human_plus_compiler_lexer_literals

#include <string>
#include <sstream>
#include <vector>

namespace hpc {
    
    namespace syntax {
        
        /*!
         \brief Class holding information about a number literal, either integer or floating point.
         */
        class NumberLiteral {
        public:
            /*!
             \brief Values indicating the possible bases literals can be expressed in.
             */
            typedef enum {
                Base2 = 2,
                Base8 = 8,
                Base10 = 10,
                Base16 = 16
            } Base;
            
        private:
            /*!
             \brief The string containing the literal, as specified in the source code.
             */
            std::string literalString;
            /*!
             \brief The string containing the suffix, as specified in the source code.
             */
            std::string suffixString;
            /*!
             \brief The base the literal is expressed in.
             */
            Base base;
            
        public:
            /*!
             \brief Initializes the object with the string containing the literal, as specified in the source code, the suffix string of the literal, and the base of the literal as inferred by the lexer.
             \note the \c literalString must not contain any prefix (0x, 0o, 0b...), as the base should be passed directly.
             */
            NumberLiteral(std::string literalString, std::string suffixString, Base base)
            : literalString(literalString), suffixString(suffixString), base(base) {  }
            
            inline Base getBase() const {
                return base;
            }
             
        };
        
        /*!
         \brief Integer type that holds parsed character values.
         */
        typedef uint32_t CharValue;
        
        /*!
         \brief The format of character literal specified in the source code.
         */
        typedef enum {
            FormatInvalid = 0, ///< Placeholder for invalid literals
            FormatAsIs    = 1, ///< The literal is specified 'as is' in the source code (e.g.: 'a')
            FormatOct     = 2, ///< The literal is specified as escaped octal number (e.g.: '\357')
            FormatHex8    = 3, ///< The literal is specified as escaped 8-bit hexadecimal number (e.g.: '\xff')
            FormatHex16   = 4, ///< The literal is specified as escaped 16-bit hexadecimal number (e.g.: '\uffff')
            FormatHex32   = 5, ///< The literal is specified as escaped 32-bit hexadecimal number (e.g.: '\Uffffffff')
        } CharSeqFormat;
        
        
        /*!
         \brief Class holding information about character data.
         */
        class CharData {
            /*!
             \brief The value of the character, as parsed by the lexer, according to the sequence specified in the source code.
             */
            CharValue value;
            /*!
             \brief The format of the literal, as specified to the source code. It is also used
             */
            CharSeqFormat format;
            
        public:
            CharData(CharValue value = 0, CharSeqFormat format = FormatAsIs) : value(value), format(format) {  }
        
            inline CharValue getValue() const {
                return value;
            }
        
            inline CharSeqFormat getFormat() const {
                return format;
            }
            
        };
        
        
        /*!
         \brief Class holding information about a character literal, of an arbitrary size.
         */
        class CharacterLiteral {
            /*!
             \brief The data about the character, as parsed by the lexer, according to the sequence specified in the source code.
             */
            CharData data;
            /*!
             \brief The string containing the suffix, as specified in the source code.
             */
            std::string suffixString;
            
            
        public:
            /*!
             \brief Initializes the object with the value and the encoding of the literal.
             */
            CharacterLiteral(CharData data, std::string suffixString) : data(data), suffixString(suffixString) {  }
            
            
            inline const CharData &detData() const {
                return data;
            }
            
            inline CharValue getValue() const {
                return data.getValue();
            }
            
            inline CharSeqFormat getFormat() const {
                return data.getFormat();
            }
            
        };
        
        /*!
         \brief Class holding information about a string literal, of an arbitrary size.
         */
        class StringLiteral {
            /*!
             \brief The value of the character, as parsed by the lexer, according to the sequence specified in the source code.
             */
            std::vector<CharData> data;
            
            
        public:
            /*!
             \brief Initializes an empty string literal.
             */
            StringLiteral() {  }
            
            
            inline const std::vector<CharData> &getData() const {
                return data;
            }
            
            void add(const CharData &cdata) {
                data.push_back(cdata);
            }
            
            inline void operator <<(const CharData &cdata) {
                add(cdata);
            }
            
        };
        
    }
}









#endif
