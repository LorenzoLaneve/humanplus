//
//  typeAST.cpp
//  Human Plus
//
//  Created by Lore on 21/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/ast/types/base.h>
#include <hpc/ast/types/builtintype.h>
#include <hpc/ast/types/pointertype.h>
#include <hpc/ast/decls/class.h>
#include <hpc/utils/printers.h>
#include <hpc/analyzers/validator/validator.h>
#include <hpc/target/target.h>

#include <llvm/ADT/StringSwitch.h>
#include <llvm/IR/DataLayout.h>

#include <sstream>

using namespace hpc;

bool ast::Type::canCastTo(Type *type, bool explicitly) {
    return !type->isConstant();
}

bool ast::Type::canAssignTo(Type *type) {
    return true;
}

ast::PointerType *ast::Type::pointerTo() {
    return ast::PointerType::get(this);
}

ast::Type *ast::Type::getPlaceholderType() {
    return BuiltinType::get(BuiltinType::SignedInteger);
}

std::string ast::Type::asString() {
    std::ostringstream os;
    
    os << str();
    if (!isCanonicalType()) {
        os << " (a.k.a. " << getCanonicalType()->str() << ")";
    }
    
    return os.str();
}

ast::QualifiedType *ast::Type::withConstantQualifier() {
    ast::QualifiedType *qualType = new ast::QualifiedType(this);
    
    qualType->setConstant(true);
    
    return qualType;
}

bool ast::Type::areEquivalent(Type &type1, Type &type2) {
    if (!type1.isCanonicalType() || !type2.isCanonicalType()) return areEquivalent(type1.getCanonicalType(), type2.getCanonicalType());
    
    if (&type1 == &type2) return true;
    
    if (type1.isConstant() != type2.isConstant()) return false;
    
    TypeEncloser *qualType1 = llvm::dyn_cast<TypeEncloser>(&type1);
    TypeEncloser *qualType2 = llvm::dyn_cast<TypeEncloser>(&type2);
    
    if (!qualType1 && !qualType2) return false;
    
    return areEquivalent(qualType1 ? qualType1->getEnclosingType() : &type1,
                         qualType2 ? qualType2->getEnclosingType() : &type2);
}


std::string ast::TypeRef::str(bool quoted) {
    assert(theType && "No type found.");
    std::ostringstream os;
    
    if (quoted) os << "'";
    os << getSymbolPath().str();
    if (quoted) os << "'";
    
    return os.str();
}

std::string ast::AliasedType::str(bool quoted) {
    std::ostringstream os;
    
    if (quoted) os << "'";
    os << declaration->getStringPath();
    if (quoted) os << "'";
    
    return os.str();
}

ast::Type *ast::QualifiedType::getCanonicalType() {
    return cloneQuals(getEnclosingType()->getCanonicalType());
}

ast::QualifiedType *ast::QualifiedType::cloneQuals(ast::Type *encType) const {
    QualifiedType *qualType = new ast::QualifiedType(encType);
    
    qualType->setConstant(isConstant());
    
    return qualType;
}

std::string ast::QualifiedType::str(bool quoted) {
    std::ostringstream os;
    
    if (quoted) os << "'";
    if (constQual) os << "constant ";
    os << getEnclosingType()->str(false);
    if (quoted) os << "'";
    
    return os.str();
}


