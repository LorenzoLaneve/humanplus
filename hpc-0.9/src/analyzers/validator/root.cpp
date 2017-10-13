//
//  root.cpp
//  HumanPlus
//
//  Created by Lore on 09/06/16.
//  Copyright © 2016 Lore. All rights reserved.
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
