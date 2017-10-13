//
//  iterationAST.cpp
//  Human Plus
//
//  Created by Lore on 13/09/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/ast/stmt/whileuntil.h>
#include <hpc/utils/printers.h>

using namespace hpc;

ast::SimpleIterStmt::SimpleIterStmt(Expr *condition, Stmt *block) : condition(new EvalExpr(condition)), block(block) {  }

bool ast::SimpleIterStmt::returns() const {
    return false;
}

int ast::SimpleIterStmt::containedReturns() const {
    if (block) return block->containedReturns();
    return 0;
}

bool ast::PostWhileStmt::returns() const {
    return block && block->returns();
}

bool ast::PostUntilStmt::returns() const {
    return block && block->returns();
}
