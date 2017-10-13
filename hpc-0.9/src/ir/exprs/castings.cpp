//
//  castings.cpp
//  HumanPlus
//
//  Created by Lore on 11/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ir/builders.h>
#include <hpc/ast/types/builtintype.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/config.h>

#include <llvm/IR/Value.h>

using namespace hpc;

void codegen::ModuleBuilder::visitImplicitCastExpr(ast::ImplicitCastExpr *cast) {
    ast::Expr *val = cast->getExpression();
    
    ast::Type *original = val->evalType();
    ast::Type *destination = cast->evalType();
    
    llvm::Value *llval = build(val);
    llvm::Type *lltype = getIRType(destination);
    
    if (original->isIntegerType() && destination->isIntegerType()) {
        if (original->isSignedIntegerType()) {
            table.setValForComponent(cast, builder->CreateSExtOrTrunc(llval, lltype));
        } else {
            table.setValForComponent(cast, builder->CreateZExtOrTrunc(llval, lltype));
        }
        return;
    }
    
    if (original->getFormat() == destination->getFormat()) {
        switch (original->getFormat()) {
            case ast::TypeFormatFloatingPoint: {
                
                ast::BuiltinType *builtinOriginal = llvm::cast<ast::BuiltinType>(original);
                ast::BuiltinType *builtinDestination = llvm::cast<ast::BuiltinType>(destination);
                
                if (builtinOriginal->getMagnitude() <= builtinDestination->getMagnitude()) {
                    table.setValForComponent(cast, builder->CreateFPExt(llval, lltype));
                } else {
                    table.setValForComponent(cast, builder->CreateFPTrunc(llval, lltype));
                }
                return;
            }
            default: break;
        }
    } else {
        if (original->isIntegerType() && destination->isFloatingPointType()) {
            if (original->isSignedIntegerType()) {
                table.setValForComponent(cast, builder->CreateSIToFP(llval, lltype));
            } else {
                table.setValForComponent(cast, builder->CreateUIToFP(llval, lltype));
            }
            return;
        }
        
        if (original->isFloatingPointType() && destination->isIntegerType()) {
            if (destination->isSignedIntegerType()) {
                table.setValForComponent(cast, builder->CreateFPToSI(llval, lltype));
            } else {
                table.setValForComponent(cast, builder->CreateFPToUI(llval, lltype));
            }
            return;
        }
    }
    
    table.setValForComponent(cast, llval); // FIXME
}

void codegen::ModuleBuilder::visitEvalExpr(ast::EvalExpr *eval) {
    ast::Expr *val = eval->getExpression();
    
    ast::Type *vtype = val->evalType();
    if (vtype->isBooleanType()) {
        table.setValForComponent(eval, builder->CreateTrunc(build(val), llvm::Type::getInt1Ty(builder->getContext())));
        return;
    }
    
#ifdef __human_plus_compiler_supports_bool_context_conversion
    if (vtype->isIntegerType()) {
        table.setValForComponent(eval, builder->CreateFCmpUNE(build(val), llvm::Constant::getNullValue(getIRType(vtype))));
        return;
    }
#endif
    //Custom Evaluations callings here
    
    table.setValForComponent(eval, builder->CreateICmpNE(build(val), llvm::Constant::getNullValue(getIRType(vtype))));
}

