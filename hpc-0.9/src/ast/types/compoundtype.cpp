//
//  compoundtype.cpp
//  HumanPlus
//
//  Created by Lore on 01/06/16.
//  Copyright © 2016 Lore. All rights reserved.
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


