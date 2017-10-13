// => src/ast/types/compoundtype.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/types/compoundtype.h>
#include <hpc/ast/decls/class.h>

#include <algorithm>
#include <sstream>

using namespace hpc;

bool ast::ClassType::canCastTo(ast::Type *type, bool explicitly) {
    return false; // FIXME
}

bool ast::ClassType::canAssignTo(ast::Type *type) {
    return false; // FIXME
}

ast::FieldDecl *ast::ClassType::getMember(std::string memberID) {
    return declaration->getFieldDecl(memberID);
}

std::string ast::ClassType::str(bool quoted) {
    std::ostringstream os;
    
    if (quoted) os << "'";
    os << declaration->getStringPath();
    if (quoted) os << "'";
    
    return os.str();
}


