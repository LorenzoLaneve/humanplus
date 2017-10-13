// => src/ast/stmt/for.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/stmt/for.h>

using namespace hpc;

ast::ForStmt::ForStmt(std::vector<Stmt *> initstmt, Expr *condition, std::vector<Stmt *> endstmt, Stmt *block)
: ast::SimpleIterStmt(condition, block), initstmt(initstmt), endstmt(endstmt) {  }
