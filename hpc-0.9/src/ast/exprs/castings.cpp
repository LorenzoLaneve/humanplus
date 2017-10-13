//
//  expressionAST.cpp
//  Human Plus
//
//  Created by Lore on 12/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/types/type.h>

#include <iostream>

using namespace hpc;

ast::Type *ast::ImplicitCastExpr::evalType() {
    return destination;
}

ast::Type *ast::EvalExpr::evalType() {
    return BuiltinType::get(BuiltinType::Boolean);
}
