//
//  types.cpp
//  HumanPlus
//
//  Created by Lore on 12/02/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#include <hpc/ir/builders.h>
#include <hpc/ast/types/base.h>
#include <hpc/ast/types/builtintype.h>
#include <hpc/ast/types/pointertype.h>
#include <hpc/ast/types/compoundtype.h>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/IR/Type.h>

using namespace hpc;

void codegen::ModuleBuilder::visitBuiltinType(ast::BuiltinType *type) {
    
    switch (type->getBuiltinTypeID()) { // FIXME this should depend on builtintypes.inc
        case ast::BuiltinType::Void:
            table.setIRType(type, llvm::Type::getVoidTy(getModule().getContext()));
            break;
        case ast::BuiltinType::Boolean:
            table.setIRType(type, llvm::Type::getInt1Ty(getModule().getContext()));
            break;
        case ast::BuiltinType::Character:
        case ast::BuiltinType::SignedByte:
        case ast::BuiltinType::UnsignedByte:
            table.setIRType(type, llvm::Type::getInt8Ty(getModule().getContext()));
            break;
        case ast::BuiltinType::UTF16Character:
        case ast::BuiltinType::SignedShort:
        case ast::BuiltinType::UnsignedShort:
            table.setIRType(type, llvm::Type::getInt16Ty(getModule().getContext()));
            break;
        case ast::BuiltinType::UTF32Character:
        case ast::BuiltinType::SignedInteger:
        case ast::BuiltinType::UnsignedInteger:
            table.setIRType(type, llvm::Type::getInt32Ty(getModule().getContext()));
            break;
        case ast::BuiltinType::SignedLong:
        case ast::BuiltinType::UnsignedLong:
            table.setIRType(type, llvm::Type::getInt64Ty(getModule().getContext()));
            break;
        case ast::BuiltinType::SignedLongLong:
        case ast::BuiltinType::UnsignedLongLong:
            table.setIRType(type, llvm::Type::getInt128Ty(getModule().getContext()));
            break;
        case ast::BuiltinType::Float:
            table.setIRType(type, llvm::Type::getFloatTy(getModule().getContext()));
            break;
        case ast::BuiltinType::Double:
            table.setIRType(type, llvm::Type::getDoubleTy(getModule().getContext()));
            break;
            
        case ast::BuiltinType::String:
            table.setIRType(type, llvm::Type::getInt8Ty(getModule().getContext())->getPointerTo());
            break;

    }
    
    //llvm_unreachable("Built-in type not recognized.");
}

void codegen::ModuleBuilder::visitPointerType(ast::PointerType *type) {
    table.setIRType(type, llvm::PointerType::get(getIRType(type->getPointedType()), 0));
}

void codegen::ModuleBuilder::visitClassType(ast::ClassType *type) {
    
    std::vector<llvm::Type *> fieldTypes;
    
    for (ast::FieldDecl *field : type->getFields()) {
        fieldTypes.push_back(getIRType(field->getType()));
    }
    
    table.setIRType(type, llvm::StructType::create(llvm::makeArrayRef(fieldTypes.data(), fieldTypes.size()),
                                                   getTargetInfo().getMangle().mangleClass(type->getDeclarator())));
}

void codegen::ModuleBuilder::visitQualifiedType(ast::QualifiedType *type) {
    table.setIRType(type, getIRType(type->getEnclosingType()));
}
