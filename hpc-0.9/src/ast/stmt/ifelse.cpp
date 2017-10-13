//
//  selectionAST.cpp
//  Human Plus
//
//  Created by Lore on 22/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/ast/stmt/ifelse.h>
#include <hpc/utils/printers.h>

using namespace hpc;

ast::IfStmt::IfStmt(Expr *condition, Stmt *thenBlock, Stmt *elseBlock) : condition(new EvalExpr(condition)), thenBlock(thenBlock), elseBlock(elseBlock) {  }

bool ast::IfStmt::returns() const {
    return thenBlock && thenBlock->returns() && elseBlock && elseBlock->returns();
}

int ast::IfStmt::containedReturns() const {
    int result = 0;
    
    if (thenBlock) result += thenBlock->containedReturns();
    if (elseBlock) result += elseBlock->containedReturns();
    
    return result;
}


