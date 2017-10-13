// => src/analyzers/validator/stmt/selection.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/ast/stmt/ifelse.h>

using namespace hpc;

void validator::ValidatorImpl::visitIfStmt(ast::IfStmt *statement) {
    
    ast::Expr *condition = statement->getCondition();
    ast::Stmt *thenBlock = statement->getThenBlock();
    ast::Stmt *elseBlock = statement->getElseBlock();
    
    validate(statement->getCondition());
    
    if (thenBlock) validate(thenBlock);
    if (elseBlock) validate(elseBlock);
    
    if (!condition->isValid() || (thenBlock && !thenBlock->isValid()) || (elseBlock && !elseBlock->isValid())) {
        statement->resignValidation();
    }
}
