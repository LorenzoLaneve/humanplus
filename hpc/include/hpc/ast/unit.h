// => hpc/ast/unit.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_tree
#define __human_plus_compiler_ast_tree

#include <hpc/utils/files.h>

#include <string>
#include <vector>
#include <map>

namespace hpc {
    namespace validator {
        struct ValidationSession;
    }
    
    namespace ast {
        
        class Decl;
        class NameSpaceDecl;
        
        class CompilationUnit;
        
        class AbstractSyntaxTree {
            
            /*!
             \brief An \c ast::NameSpaceDecl object holding the main global scope for the AST.
             */
            NameSpaceDecl *globalScope;
            
            /*!
             \brief A map containing all the compilation units associated to this AST.
             */
            std::map<fsys::InputFile *, ast::CompilationUnit *> compilationUnits;
            /*!
             \brief A vector containing all the compilation units associated to this AST.
             */
            std::vector<ast::CompilationUnit *> unitsVector; // FIXME redundant storage
            
        public:
            AbstractSyntaxTree();
            
            /*!
             \brief Returns an \c ast::NameSpaceDecl object holding the main global scope for the current AST.
             */
            inline NameSpaceDecl *getRootNameSpace() const {
                return globalScope;
            }
            
            inline const std::vector<ast::CompilationUnit *> &getAllUnits() const {
                return unitsVector;
            }
            
            void addUnit(ast::CompilationUnit *theUnit);
            
            ast::CompilationUnit *getUnitForFile(fsys::InputFile *file);
            

            inline ast::CompilationUnit *operator[](fsys::InputFile *inputFile) {
                return getUnitForFile(inputFile);
            }
        };
        
        /*!
         \brief Class implementing an AST Unit from a specific source file.
         */
        class CompilationUnit {
            
            /*!
             \brief A vector of top-level declarations declared in the file associated to this unit.
             */
            std::vector<ast::Decl *> topLevelDecls;
            
            /*!
             \brief The input file this AST has been created from or \c nullptr.
             */
            fsys::InputFile *associatedFile;
            
        public:
            /*!
             \brief Initializes the AST root with the input file this AST Unit is created from.
             \note If no input file is specified, then the standard input will be deducted as input file associated to the given AST unit.
             */
            CompilationUnit(fsys::InputFile *associatedFile = nullptr);
            
            /*!
             \brief Returns a vector containing all the top level declarations contained in this unit.
             */
            inline const std::vector<ast::Decl *> &getTopLevelDeclarations() const {
                return topLevelDecls;
            }
            
            /*!
             \brief Returns the input file this unit is generated from.
             */
            inline fsys::InputFile *getAssociatedFile() const {
                return associatedFile;
            }
            
            void addTopLevelDecl(ast::Decl *decl) {
                topLevelDecls.push_back(decl);
            }
            
        };

    }
}


#endif
