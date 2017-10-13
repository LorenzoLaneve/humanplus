// => src/analyzers/validator/root.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/ast/unit.h>
#include <hpc/ast/component.h>
#include <hpc/ast/decls/namespace.h>

using namespace hpc;

void validator::ValidatorImpl::visitAST(ast::AbstractSyntaxTree &ast) {
    
    resolver = new SymbolResolver(validator, ast);
    
    takeDecl(ast.getRootNameSpace());
    
}

void validator::ValidatorImpl::visitUnit(ast::CompilationUnit &unit) {
    
    for (ast::Decl *topLevelDecl : unit.getTopLevelDeclarations()) {
        takeDecl(topLevelDecl);
    }
    
}
