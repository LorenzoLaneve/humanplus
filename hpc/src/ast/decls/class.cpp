// => src/ast/decls/class.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/decls/class.h>
#include <hpc/ast/types/compoundtype.h>
#include <hpc/utils/printers.h>
#include <hpc/utils/strings.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/analyzers/validator/validator.h>

#include <llvm/IR/Value.h>

using namespace hpc;

ast::ClassDecl::ClassDecl(std::string name, std::string base, std::vector<std::string> protocols)
: NameSpaceDecl(name), base(base), protocols(protocols), classType(new ast::ClassType(this)) {  }

ast::FieldDecl *ast::ClassDecl::getFieldDecl(std::string memberid) {
    return fields[memberid];
}

void ast::ClassDecl::addField(ast::FieldDecl *field) {
    field->setFieldIndex(fieldv.size());
    
    members.push_back(field);
    fieldv.push_back(field);
    
    
    // FIXME quite unsafe
    if (!fields[field->getName()]) fields[field->getName()] = field;
}


ast::FieldDecl::FieldDecl(std::string name, Type *type, ClassDecl *container, Expr *initval)
: GlobalVar(name, type, container, initval), containerClass(container) {  }

ast::ClassType *ast::FieldDecl::getEnclosingType() {
    return containerClass->getType();
}

void ast::FieldDecl::setFieldIndex(index_t index) {
    this->index = index;
}
