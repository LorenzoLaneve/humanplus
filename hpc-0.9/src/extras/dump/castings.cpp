//
//  castings.cpp
//  HumanPlus
//
//  Created by Lore on 09/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/extras/dump/dump.h>

using namespace hpc;

void extras::NewASTPrinter::visitImplicitCastExpr(ast::ImplicitCastExpr *expression) {
    printObject("ImplicitCastExpr", expression);
    takeType(expression->getDestination());
    os << "\n";
    
    openChildBranch(1);
    takeStmt(expression->getExpression());
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitEvalExpr(ast::EvalExpr *cast) {
    printObject("ImplicitCastExpr", cast);
    os << "'bool' \n";
    
    openChildBranch(1);
    takeStmt(cast->getExpression());
    closeLastChildBranch();
}

