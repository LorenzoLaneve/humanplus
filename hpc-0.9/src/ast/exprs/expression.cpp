//
//  valueAST.cpp
//  Human Plus
//
//  Created by Lore on 12/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/ast/exprs/expression.h>
#include <hpc/analyzers/sources.h>

using namespace hpc;

source::TokenRef *ast::Expr::completeRef() {
    return source::TokenRef::join(tokenRef(PointToBeginOfExpression), tokenRef(PointToEndOfExpression));
}



