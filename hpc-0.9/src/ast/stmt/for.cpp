//
//  for.cpp
//  HumanPlus
//
//  Created by Lore on 20/05/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ast/stmt/for.h>

using namespace hpc;

ast::ForStmt::ForStmt(std::vector<Stmt *> initstmt, Expr *condition, std::vector<Stmt *> endstmt, Stmt *block)
: ast::SimpleIterStmt(condition, block), initstmt(initstmt), endstmt(endstmt) {  }
