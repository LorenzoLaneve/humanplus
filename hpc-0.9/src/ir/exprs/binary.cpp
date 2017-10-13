//
//  literals.cpp
//  HumanPlus
//
//  Created by Lore on 10/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ir/builders.h>
#include <hpc/ast/exprs/binary.h>

#include <llvm/IR/Value.h>

using namespace hpc;

static bool hasNSW(ast::BinaryExpr &expression) {
    ast::Type *lhsTy = expression.getLHS()->evalType();
    ast::Type *rhsTy = expression.getRHS()->evalType();
    
    return lhsTy->isSignedIntegerType() || rhsTy->isSignedIntegerType(); // FIXME
}


void codegen::ModuleBuilder::visitArithmeticExpr(ast::ArithmeticExpr *expression) {
    llvm::Value *L = build(expression->getLHS());
    llvm::Value *R = build(expression->getRHS());
    
    ast::Type *type = expression->evalType();
    
    bool NUW = false;
    bool NSW = hasNSW(*expression);
    
    switch (expression->getOperator()) {
        case lexer::TokenOperatorPlus: {
            if (type->isIntegerType()) {
                table.setValForComponent(expression, builder->CreateAdd(L, R, "", NUW, NSW));
                return;
            }
            if (type->isFloatingPointType()) {
                table.setValForComponent(expression, builder->CreateFAdd(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorMinus: {
            if (type->isIntegerType()) {
                table.setValForComponent(expression, builder->CreateSub(L, R, "", NUW, NSW));
                return;
            }
            if (type->isFloatingPointType()) {
                table.setValForComponent(expression, builder->CreateFSub(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorMultiply: {
            if (type->isIntegerType()) {
                table.setValForComponent(expression, builder->CreateMul(L, R, "", NUW, NSW));
                return;
            }
            if (type->isFloatingPointType()) {
                table.setValForComponent(expression, builder->CreateFMul(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorDivide: {
            if (type->isIntegerType()) {
                // TODO divide by zero exception throwing
                if (NSW) {
                    table.setValForComponent(expression, builder->CreateSDiv(L, R));
                } else {
                    table.setValForComponent(expression, builder->CreateUDiv(L, R));
                }
                return;
            }
            if (type->isFloatingPointType()) {
                // TODO divide by zero exception throwing
                table.setValForComponent(expression, builder->CreateFDiv(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorRemainder: {
            if (type->isIntegerType()) {
                // TODO remainder by zero exception throwing
                if (NSW) {
                    table.setValForComponent(expression, builder->CreateSRem(L, R));
                } else {
                    table.setValForComponent(expression, builder->CreateURem(L, R));
                }
                return;
            }
            if (type->isFloatingPointType()) {
                // TODO remainder by zero exception throwing
                table.setValForComponent(expression, builder->CreateFRem(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
    }
}

void codegen::ModuleBuilder::visitComparisonExpr(ast::ComparisonExpr *expression) {
    llvm::Value *L = build(expression->getLHS());
    llvm::Value *R = build(expression->getRHS());
    
    ast::Type *type = expression->evalType();
    
    bool hasSign = hasNSW(*expression);
    
    switch (expression->getOperator()) {
        case lexer::TokenOperatorLower: {
            if (type->isIntegerType()) {
                if (hasSign) {
                    table.setValForComponent(expression, builder->CreateICmpSLT(L, R));
                } else {
                    table.setValForComponent(expression, builder->CreateICmpULT(L, R));
                }
                return;
            }
            if (type->isFloatingPointType()) {
                table.setValForComponent(expression, builder->CreateFCmpOLT(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorLowerEqual: {
            if (type->isIntegerType()) {
                if (hasSign) {
                    table.setValForComponent(expression, builder->CreateICmpSLE(L, R));
                } else {
                    table.setValForComponent(expression, builder->CreateICmpULE(L, R));
                }
                return;
            }
            if (type->isFloatingPointType()) {
                table.setValForComponent(expression, builder->CreateFCmpOLE(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorGreater: {
            if (type->isIntegerType()) {
                if (hasSign) {
                    table.setValForComponent(expression, builder->CreateICmpSGT(L, R));
                } else {
                    table.setValForComponent(expression, builder->CreateICmpUGT(L, R));
                }
                return;
            }
            if (type->isFloatingPointType()) {
                table.setValForComponent(expression, builder->CreateFCmpOGT(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorGreaterEqual: {
            if (type->isIntegerType()) {
                if (hasSign) {
                    table.setValForComponent(expression, builder->CreateICmpSGE(L, R));
                } else {
                    table.setValForComponent(expression, builder->CreateICmpUGE(L, R));
                }
                return;
            }
            if (type->isFloatingPointType()) {
                table.setValForComponent(expression, builder->CreateFCmpOGE(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorEqual: {
            if (type->isIntegerType()) {
                table.setValForComponent(expression, builder->CreateICmpEQ(L, R));
                return;
            }
            if (type->isFloatingPointType()) {
                table.setValForComponent(expression, builder->CreateFCmpOEQ(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorNotEqual: {
            if (type->isIntegerType()) {
                table.setValForComponent(expression, builder->CreateICmpNE(L, R));
                return;
            }
            if (type->isFloatingPointType()) {
                table.setValForComponent(expression, builder->CreateFCmpONE(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
    }
}

void codegen::ModuleBuilder::visitAssignmentExpr(ast::AssignmentExpr *expression) {
    llvm::Value *R = build(expression->getRHS());
    
    ast::Type *assignmentType = expression->evalType();
    
    if (assignmentType->isBooleanType()) {
        R = builder->CreateZExt(R, getIRType(assignmentType));
    }
    
    table.setValForComponent(expression, builder->CreateStore(R, table.getOrCreateReference(expression->getLHS())));
}

void codegen::ModuleBuilder::visitBitwiseExpr(ast::BitwiseExpr *expression) {
    llvm::Value *L = build(expression->getLHS());
    llvm::Value *R = build(expression->getRHS());
    
    ast::Type *leftHandType = expression->getLHS()->evalType();
    ast::Type *rightHandType = expression->getRHS()->evalType();
    
    switch (expression->getOperator()) {
        case lexer::TokenOperatorLeftShift: {
            if (leftHandType->isIntegerType() && rightHandType->isIntegerType()) {
                table.setValForComponent(expression, builder->CreateShl(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorRightShift: {
            if (leftHandType->isIntegerType() && rightHandType->isIntegerType()) {
                
                if (leftHandType->isSignedIntegerType()) {
                    table.setValForComponent(expression, builder->CreateAShr(L, R));
                    return;
                }
                
                table.setValForComponent(expression, builder->CreateLShr(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorBitwiseAnd: {
            if (leftHandType->isIntegerType() && rightHandType->isIntegerType()) {
                table.setValForComponent(expression, builder->CreateAnd(L, R));
                return;
            }
            
            return; // TODO operator overloadings
        }
        case lexer::TokenOperatorBitwiseOr: {
            if (leftHandType->isIntegerType() && rightHandType->isIntegerType()) {
                table.setValForComponent(expression, builder->CreateOr(L, R));
                return;
            }

            return; // TODO operator overloadings
        }
    }
}

