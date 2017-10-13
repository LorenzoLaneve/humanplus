// => src/ast/types/pointertype.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/types/pointertype.h>
#include <hpc/target/target.h>
#include <hpc/utils/printers.h>

#include <map>
#include <sstream>

using namespace hpc;

bool ast::PointerType::canCastTo(ast::Type *type, bool explicitly) {
    
    if (type->isBooleanType()) {
        return true;
    }
    
    return false;
}

bool ast::PointerType::canAssignTo(ast::Type *type) {
    
    if (type->isBooleanType()) {
        return true;
    }
    
    if (type->isPointerType()) {
        return !pointedType || Type::areEquivalent(pointedType, type->getPointedType());
    }
    
    return false;
}

std::string ast::PointerType::str(bool quoted) {
    std::ostringstream os;
    
    if (quoted) os << "'";
    os << pointedType->str(false) << "<-";
    if (quoted) os << "'";
    
    return os.str();
}

ast::PointerType *ast::PointerType::get(ast::Type *type) {
    
    static std::map<ast::Type *, ast::PointerType *> pointerTypes;
    
    if (llvm::isa<QualifiedType>(type)) {
        return new PointerType(type);
    }
    
    if (!pointerTypes[type]) {
        pointerTypes[type] = new PointerType(type);
    }
    
    return pointerTypes[type];
}

