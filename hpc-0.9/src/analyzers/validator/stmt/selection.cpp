//
//  selection.cpp
//  HumanPlus
//
//  Created by Lore on 09/06/16.
//  Copyright © 2016 Lore. All rights reserved.
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
