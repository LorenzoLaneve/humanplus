//
//  constant.cpp
//  HumanPlus
//
//  Created by Lore on 14/05/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ast/exprs/constant.h>
#include <hpc/ast/types/builtintype.h>
#include <hpc/ast/types/pointertype.h>

#include <llvm/IR/Constants.h>

#include <iostream>
#include <vector>

using namespace hpc;

ast::Type *ast::CharLiteral::evalType() {
    return BuiltinType::get(BuiltinType::Character);
}

ast::Type *ast::IntegerLiteral::evalType() {
    return BuiltinType::get(BuiltinType::SignedInteger);
}

ast::Type *ast::UIntegerLiteral::evalType() {
    return BuiltinType::get(BuiltinType::UnsignedInteger);
}

ast::Type *ast::LongLiteral::evalType() {
    return BuiltinType::get(BuiltinType::SignedLong);
}

ast::Type *ast::ULongLiteral::evalType() {
    return BuiltinType::get(BuiltinType::UnsignedLong);
}

ast::Type *ast::StringLiteral::evalType() {
    return PointerType::get(BuiltinType::get(BuiltinType::Character)->withConstantQualifier());
}

ast::Type *ast::FloatLiteral::evalType() {
    return BuiltinType::get(BuiltinType::Float);
}

ast::Type *ast::DoubleLiteral::evalType() {
    return BuiltinType::get(BuiltinType::Double);
}

ast::Type *ast::BoolLiteral::evalType() {
    return BuiltinType::get(BuiltinType::Boolean);
}

ast::Type *ast::NullPointer::evalType() {
    return PointerType::get(nullptr);
}
