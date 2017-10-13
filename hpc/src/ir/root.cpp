// => src/ir/root.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ir/modules.h>
#include <hpc/ir/builders.h>
#include <hpc/ast/unit.h>
#include <hpc/ast/decls/namespace.h>

#include <llvm/IR/Value.h>

using namespace hpc;

void codegen::ModuleBuilder::visitUnit(ast::CompilationUnit &unit) {
    
    for (ast::Decl *decl : unit.getTopLevelDeclarations()) {
        takeDecl(decl);
    }

}
