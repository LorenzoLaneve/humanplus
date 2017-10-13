//
//  unaryexpr.cpp
//  HumanPlus
//
//  Created by Lore on 08/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ast/exprs/unary.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/types/base.h>
#include <hpc/utils/printers.h>
#include <hpc/target/target.h>

using namespace hpc;

void ast::UnaryExpr::castOperandToBoolean() {
    exp = new ast::EvalExpr(exp);
}

ast::Type *ast::ArithmeticNegationExpr::evalType() {
    ast::Type *operandTy = exp->evalType();
    
    if (operandTy->isUnsignedIntegerType()) {
        return operandTy; // FIXME this should be signed type
    }
    
    return operandTy;
}

ast::Type *ast::LogicalNegationExpr::evalType() {
    return exp->evalType();
}

ast::Type *ast::BitwiseNegationExpr::evalType() {
    return exp->evalType();
}
