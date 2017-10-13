//
//  BuiltinType.cpp
//  Human Plus
//
//  Created by Lore on 22/11/15.
//  Copyright © 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/ast/types/builtintype.h>
#include <hpc/utils/printers.h>

#include <map>
#include <sstream>

using namespace hpc;

ast::BuiltinType::TypeMagnitude ast::BuiltinType::getMagnitude() const {
    switch (typeID) {
            
#define __builtintype(ID, SRCID, FORMAT, MAGNITUDE) case ID: return MAGNITUDE;
#include <hpc/inc/builtintypes.inc>
#undef __builtintype
            
    }
    llvm_unreachable("Undefined built-in type.");
}


std::string ast::BuiltinType::getIdentifier() const {
    switch (typeID) {
            
#define __builtintype(ID, SRCID, FORMAT, MAGNITUDE) case ID: return SRCID;
#include <hpc/inc/builtintypes.inc>
#undef __builtintype
            
    }
    llvm_unreachable("Undefined built-in type.");
}

ast::TypeFormat ast::BuiltinType::getFormat() const {
    switch (typeID) {
            
#define __builtintype(ID, SRCID, FORMAT, MAGNITUDE) case ID: return TypeFormat##FORMAT;
#include <hpc/inc/builtintypes.inc>
#undef __builtintype
            
    }
    llvm_unreachable("Undefined built-in type.");
}

ast::BuiltinType *ast::BuiltinType::get(TypeID typeID) {
    
    static std::map<TypeID, BuiltinType *> builtinTypes;
    
    if (!builtinTypes[typeID]) {
        builtinTypes[typeID] = new BuiltinType(typeID);
    }
    
    return builtinTypes[typeID];
}

bool ast::BuiltinType::canCastTo(Type *type, bool explicitly) {
    assert(type && "Unknown type 'null'.");
    
    if (isVoidType() || type->isVoidType()) return false;
    
    if (Type::areEquivalent(this, type)) return true;
    
    if (type->isBooleanType()) {
        return isNumericType(); // this is a built-in type, it can't be a pointer.
    }
    
    if (isIntegerType() && type->isIntegerType()) return true;
    
    if (getFormat() == type->getFormat()) {
        if (isNumericType()) return true;
        // FIXME this may harm the language safetiness. Also think about exception throwing on overflow to allow such casts.
        
        const ast::BuiltinType *btype = llvm::cast<const ast::BuiltinType>(type);
        
        return explicitly || getMagnitude() <= btype->getMagnitude();
    }
    
    if (isIntegerType() && type->isFloatingPointType()) return true;
    // Integer types can always be casted to floating point types.
    
    if (isFloatingPointType() && type->isIntegerType()) return explicitly;
    // Floating point types can be casted only if the user explicitly specifies the cast.
    
    return false;
}

bool ast::BuiltinType::canAssignTo(ast::Type *type) {
    if (isVoidType() || type->isVoidType()) return false;
    
    if (Type::areEquivalent(this, type)) return true;
    if (isIntegerType() && type->isIntegerType()) return true;
    if (getFormat() == type->getFormat()) return true;
    
    if (isIntegerType() && type->isFloatingPointType()) return true;
    if (type->isIntegerType() && isFloatingPointType()) return true;
    
    return false;
}

std::string ast::BuiltinType::str(bool quoted) {
    std::ostringstream os;
    
    if (quoted) os << "'";
    os << getIdentifier();
    if (quoted) os << "'";
    
    return os.str();
}
