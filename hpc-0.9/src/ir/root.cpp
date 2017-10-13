//
//  root.cpp
//  HumanPlus
//
//  Created by Lore on 10/06/16.
//  Copyright © 2016 Lore. All rights reserved.
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
