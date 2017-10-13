// => src/ast/unit.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/unit.h>
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
