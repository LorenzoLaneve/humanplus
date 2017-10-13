// => src/ast/stmt/ifelse.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
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


