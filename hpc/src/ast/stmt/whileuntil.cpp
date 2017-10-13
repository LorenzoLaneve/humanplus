// => src/ast/stmt/whileuntil.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
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
