//
//  selection.cpp
//  HumanPlus
//
//  Created by Lore on 11/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ir/builders.h>
#include <hpc/ast/stmt/ifelse.h>

#include <llvm/IR/Value.h>

using namespace hpc;

void codegen::ModuleBuilder::visitIfStmt(ast::IfStmt *statement) {
    llvm::Function *irfunc = builder->GetInsertBlock()->getParent();
    
    llvm::BasicBlock *trueBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *falseBlock = nullptr;
    llvm::BasicBlock *remergeBlock = llvm::BasicBlock::Create(builder->getContext());
    
    if (statement->hasElseBlock())
        falseBlock = llvm::BasicBlock::Create(builder->getContext());
    
    
    builder->CreateCondBr(build(statement->getCondition()), trueBlock, statement->hasElseBlock() ? falseBlock : remergeBlock);
    irfunc->getBasicBlockList().push_back(trueBlock);
    
    if (ast::Stmt *thenBlock = statement->getThenBlock()) {
        builder->SetInsertPoint(trueBlock);
        build(thenBlock);
    }
    builder->CreateTerminatorIfNeeded(remergeBlock);
    
    
    if (ast::Stmt *elseBlock = statement->getElseBlock()) {
        irfunc->getBasicBlockList().push_back(falseBlock);
        builder->SetInsertPoint(falseBlock);
        build(elseBlock);
        builder->CreateTerminatorIfNeeded(remergeBlock);
    }
    
    irfunc->getBasicBlockList().push_back(remergeBlock);
    builder->SetInsertPoint(remergeBlock);
}
