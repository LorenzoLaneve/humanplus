// => hpc/ast/symbols.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_symbols
#define __human_plus_compiler_ast_symbols

#include <hpc/ast/unit.h>
#include <hpc/ast/component.h>
#include <hpc/analyzers/sources.h>

#include <string>
#include <vector>

namespace hpc {
    namespace ast {
        
        /*!
         \brief Structure which defines an identifier contained in an \c ast::Symbol object.
         \param identitier The unqualified identifier parsed by the parser
         \param symref A \c source::SrcLoc pointing to the identifier in the source code
         */
        typedef struct {
            std::string identifier;
            source::SrcLoc *symref;
        } SymbolIdentifier;
        
        /*!
         \brief It describes a Symbol described in the source code as \c id1::id2::id3, which can refer to another AST entity.
         */
        class Symbol : public Component {
            /*!
             \brief An array of \c SymbolIdentifier containing the identifiers that form the Symbol.
             */
            std::vector<SymbolIdentifier> sympath;
            
        public:
            /*!
             \brief Makes an empty and invalid Symbol, with no identifiers.
             */
            Symbol() {  }
            /*!
             \brief Makes a new \c ast::Symbol, initializing it with the members for a first \c SymbolIdentifier object.
             \param symroot A string containing the first identifier
             \param tkref A \c source::SrcLoc object pointing to the identifier in the source file.
             \see \c SymbolIdentifier for the structure in which the costructor arguments will be stored in the \c ast::Symbol object.
             */
            Symbol(std::string symroot, source::SrcLoc *tkref = nullptr);
            
            
            Symbol(ast::SymbolIdentifier &symbolID);
            virtual ~Symbol() {  }
            
            /*!
             \brief Returns a \c std::vector containing all the \c SymbolIdentifier objects representing the whole Symbol.
             */
            inline const std::vector<SymbolIdentifier> &extract() const {
                return sympath;
            }
            
            /*!
             \brief Returns a boolean indicating whether the Symbol has more than one identifier. If there is more than one identifier, only the last identifier will refer to the entity, while the previous identifier will refer to container entities for the entity.
             \return \c true if there is more than one identifier composing the Symbol, \c false otherwise.
             */
            inline bool isNested() const {
                return sympath.size() > 1;
            }
            
            /*!
             \brief Returns whether the Symbol object is valid and may refer to an entity.
             */
            inline bool isValid() const {
                // FIXME symbols starting with ::
                return sympath.size() > 0 && sympath[0].identifier.length();
            }
            
            /*!
             \brief Returns the \c SymbolIdentifier describing the first identifier contained by the Symbol.
             \see \c SymbolIdentifier A struct containing an identifier in a Symbol object.
             */
            const SymbolIdentifier &getRootIdentifier() const {
                return sympath.front();
            }
            /*!
             \brief Returns the \c SymbolIdentifier describing the last identifier contained by the Symbol.
             \see \c SymbolIdentifier A struct containing an identifier in a Symbol object.
             */
            const SymbolIdentifier &getTopIdentifier() const {
                return sympath.back();
            }
            
            /*!
             \brief Returns an \c ast::Symbol object describing a copy of this Symbol without the first identifier.
             */
            Symbol containedSymbol() const;
            /*!
             \brief Returns an \c ast::Symbol object describing a copy of this Symbol without the last identifier.
             */
            Symbol containerSymbol() const;
            
            /*!
             \brief Adds a new identifier to the chain of the Symbol as last element.
             \param childsym A string containing the parsed unqualified identifier
             \param tkref A \c source::SrcLoc object pointing to the identifier in the source code
             */
            void pushBackChild(std::string childsym, source::SrcLoc *tkref = nullptr);
            
            /*!
             \brief Adds a new identifier to the chain of the Symbol as first element.
             \param childsym A string containing the parsed unqualified identifier
             \param tkref A \c source::SrcLoc object pointing to the identifier in the source code
             */
            void pushFrontChild(std::string childsym, source::SrcLoc *tkref = nullptr);
            
            /*!
             \brief Returns a string containing the identifier in the Symbol at given index (starting from 0).
             */
            inline std::string &operator[](unsigned int i) {
                return sympath[i].identifier;
            }
            
            /*!
             \brief Returns a \c std::string describing the AST object.
             */
            virtual std::string str() const;
            
            llvm_rtti_impl(Symbol);
        };
    }
}

#endif
