// => hpc/ast/files/writer.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_files_writer
#define __human_plus_compiler_ast_files_writer

#include <hpc/ast/ast.h>
#include <hpc/ast/visitor.h>

#include <fstream>

namespace hpc {
    namespace ast {
        
        class ASTWriter : ast::RecursiveVisitor<ASTWriter> {

            ast::AbstractSyntaxTree &ast;
            
            std::ofstream *outs;

        public:
            /*!
             \brief Initializes an AST file writer object ready to write the given AST to the file system.
             */
            ASTWriter(ast::AbstractSyntaxTree &ast);

            /*!
             \brief
             */
            inline std::ofstream &fouts() const {
                assert(outs && "No stream opened.");
                return *outs;
            }

            /*!
             \brief Writes the AST into the given filePath. The filePath will be the folder which will contain the AST.
             \return \c true if the AST was written successfully to the file system, \c false if an error occurred.
             */
            bool writeTo(std::string filePath);

        };
        
    }
}

#endif
