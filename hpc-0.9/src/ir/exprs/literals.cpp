//
//  literals.cpp
//  HumanPlus
//
//  Created by Lore on 10/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ir/builders.h>
#include <hpc/ast/exprs/constant.h>

#include <llvm/IR/Value.h>
#include <llvm/IR/Constant.h>

using namespace hpc;

void codegen::ModuleBuilder::visitCharLiteral(ast::CharLiteral *literal) {
    table.setValForComponent(literal, llvm::ConstantInt::get(getIRType(literal->evalType()), literal->getValue()));
}

void codegen::ModuleBuilder::visitIntegerLiteral(ast::IntegerLiteral *literal) {
    table.setValForComponent(literal, llvm::ConstantInt::get(getIRType(literal->evalType()), literal->getValue(), /*isSigned=*/true));
}

void codegen::ModuleBuilder::visitUIntegerLiteral(ast::UIntegerLiteral *literal) {
    table.setValForComponent(literal, llvm::ConstantInt::get(getIRType(literal->evalType()), literal->getValue(), /*isSigned=*/false));
}

void codegen::ModuleBuilder::visitLongLiteral(ast::LongLiteral *literal) {
    table.setValForComponent(literal, llvm::ConstantInt::get(getIRType(literal->evalType()), literal->getValue(), /*isSigned=*/true));
}

void codegen::ModuleBuilder::visitULongLiteral(ast::ULongLiteral *literal) {
    table.setValForComponent(literal, llvm::ConstantInt::get(getIRType(literal->evalType()), literal->getValue(), /*isSigned=*/false));
}

void codegen::ModuleBuilder::visitFloatLiteral(ast::FloatLiteral *literal) {
    table.setValForComponent(literal, llvm::ConstantFP::get(getIRType(literal->evalType()), literal->getValue()));
}

void codegen::ModuleBuilder::visitDoubleLiteral(ast::DoubleLiteral *literal) {
    table.setValForComponent(literal, llvm::ConstantFP::get(getIRType(literal->evalType()), literal->getValue()));
}

void codegen::ModuleBuilder::visitBoolLiteral(ast::BoolLiteral *literal) {
    table.setValForComponent(literal, literal->getValue() ?
                       llvm::ConstantInt::getTrue(builder->getContext()) :
                       llvm::ConstantInt::getFalse(builder->getContext())
                      );
}

void codegen::ModuleBuilder::visitStringLiteral(ast::StringLiteral *literal) {
    llvm::GlobalVariable *stringConstant = table.getOrCreateStringConstant(literal->getValue());
    
    table.setValForComponent(literal, builder->CreateConstInBoundsGEP2_32(stringConstant->getValueType(), stringConstant, 0, 0));
}

void codegen::ModuleBuilder::visitNullPointer(ast::NullPointer *nullpointer) {
    table.setValForComponent(nullpointer, llvm::Constant::getNullValue(getIRType(nullpointer->getPointerType())));
}
