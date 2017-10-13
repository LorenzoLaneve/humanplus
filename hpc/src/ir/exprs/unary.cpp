// => src/ir/exprs/unary.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ir/builders.h>
#include <hpc/target/target.h>
#include <hpc/ast/types/type.h>
#include <hpc/ast/exprs/unary.h>

#include <llvm/IR/Value.h>

using namespace hpc;

void codegen::ModuleBuilder::visitArithmeticNegationExpr(ast::ArithmeticNegationExpr *expression) {
    ast::Expr *operand = expression->getOperand();
    
    ast::Type *operandType = operand->evalType();
    
    if (operandType->isFloatingPointType()) {
        table.setValForComponent(expression, builder->CreateFNeg(build(operand)));
        return;
    } else if (operandType->isIntegerType()) {
        if (static_cast<ast::BuiltinType *>(operandType)->getMagnitude() < 4 || operandType->isSignedIntegerType()) { // FIXME anything more meaningful?
            table.setValForComponent(expression, builder->CreateNSWNeg(build(operand)));
        } else {
            table.setValForComponent(expression, builder->CreateNeg(build(operand)));
        }
        return;
    }
    
    // TODO operator overloading
}

void codegen::ModuleBuilder::visitLogicalNegationExpr(ast::LogicalNegationExpr *expression) {
    table.setValForComponent(expression, builder->CreateNot(build(expression->getOperand())));
    // TODO operator overloading
}

void codegen::ModuleBuilder::visitBitwiseNegationExpr(ast::BitwiseNegationExpr *expression) {
    table.setValForComponent(expression, builder->CreateNot(build(expression->getOperand())));
    // TODO operator overloading
}
