//
//  ast.cpp
//  Human Plus
//
//  Created by Lore on 08/03/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/ast/unit.h>
//#include <hpc/ast/visitor.h>
#include <hpc/ast/decls/namespace.h>
#include <hpc/analyzers/validator/validator.h>
#include <hpc/ir/builders.h>

using namespace hpc;

ast::AbstractSyntaxTree::AbstractSyntaxTree() : globalScope(new NameSpaceDecl()) {  }

void ast::AbstractSyntaxTree::addUnit(ast::CompilationUnit *theUnit) {
    assert(getUnitForFile(theUnit->getAssociatedFile()) && "There is already a unit associated to the given file.");
    compilationUnits[theUnit->getAssociatedFile()] = theUnit;
    unitsVector.push_back(theUnit);
}

ast::CompilationUnit *ast::AbstractSyntaxTree::getUnitForFile(fsys::InputFile *file) {
    return compilationUnits[file];
}


ast::CompilationUnit::CompilationUnit(fsys::InputFile *associatedFile) : associatedFile(associatedFile) {  }
