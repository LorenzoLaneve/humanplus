// => src/extras/dump/castings.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
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

