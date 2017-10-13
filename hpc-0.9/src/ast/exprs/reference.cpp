//
//  reference.cpp
//  HumanPlus
//
//  Created by Lore on 14/05/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ast/exprs/reference.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/analyzers/validator/validator.h>

using namespace hpc;

ast::Type *ast::VarRef::evalType() {
    if (!variable) return nullptr;
    return variable->getType();
}


void ast::FunctionCall::castActualParamToType(int i, ast::Type *destination) {
    actualParams[i] = new ast::ImplicitCastExpr(actualParams[i], destination);
}

ast::Type *ast::FunctionCall::evalType() {
    if (!prototype) return nullptr;
    return prototype->getReturnType();
}

void ast::FunctionCall::addCallParameter(Expr *parameter) {
    actualParams.push_back(parameter);
}
