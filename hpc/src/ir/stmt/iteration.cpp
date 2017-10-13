// => src/ir/stmt/iteration.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ir/builders.h>
#include <hpc/ast/stmt/whileuntil.h>
#include <hpc/ast/stmt/for.h>

#include <llvm/IR/Value.h>

using namespace hpc;

void codegen::ModuleBuilder::visitPreWhileStmt(ast::PreWhileStmt *statement) {
    llvm::Function *irfunc = builder->GetInsertBlock()->getParent();
    
    llvm::BasicBlock *conditionBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *iterationBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *remergeBlock = llvm::BasicBlock::Create(builder->getContext());
    
    builder->CreateBr(conditionBlock);
    irfunc->getBasicBlockList().push_back(conditionBlock);
    
    builder->SetInsertPoint(conditionBlock);
    builder->CreateCondBr(build(statement->getCondition()), iterationBlock, remergeBlock);
    irfunc->getBasicBlockList().push_back(iterationBlock);
    
    
    builder->SetInsertPoint(iterationBlock);
    if (ast::Stmt *block = statement->getBlock()) {
        build(block);
    }
    builder->CreateTerminatorIfNeeded(conditionBlock);
    
    irfunc->getBasicBlockList().push_back(remergeBlock);
    builder->SetInsertPoint(remergeBlock);
    
    table.setBreakBlock(*statement, remergeBlock);
    table.setContinueBlock(*statement, conditionBlock);
}

void codegen::ModuleBuilder::visitPreUntilStmt(ast::PreUntilStmt *statement) {
    llvm::Function *irfunc = builder->GetInsertBlock()->getParent();
    
    llvm::BasicBlock *conditionBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *iterationBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *remergeBlock = llvm::BasicBlock::Create(builder->getContext());
    
    builder->CreateBr(conditionBlock);
    irfunc->getBasicBlockList().push_back(conditionBlock);
    
    builder->SetInsertPoint(conditionBlock);
    builder->CreateCondBr(build(statement->getCondition()), remergeBlock, iterationBlock);
    irfunc->getBasicBlockList().push_back(iterationBlock);
    
    
    builder->SetInsertPoint(iterationBlock);
    if (ast::Stmt *block = statement->getBlock()) {
        build(block);
    }
    builder->CreateTerminatorIfNeeded(conditionBlock);
    
    irfunc->getBasicBlockList().push_back(remergeBlock);
    builder->SetInsertPoint(remergeBlock);
    
    table.setBreakBlock(*statement, remergeBlock);
    table.setContinueBlock(*statement, conditionBlock);
}

void codegen::ModuleBuilder::visitPostWhileStmt(ast::PostWhileStmt *statement) {
    llvm::Function *irfunc = builder->GetInsertBlock()->getParent();
    
    llvm::BasicBlock *iterationBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *conditionBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *remergeBlock = llvm::BasicBlock::Create(builder->getContext());
    
    builder->CreateBr(iterationBlock);
    irfunc->getBasicBlockList().push_back(iterationBlock);
    
    builder->SetInsertPoint(iterationBlock);
    if (ast::Stmt *block = statement->getBlock()) {
        build(block);
    }
    builder->CreateTerminatorIfNeeded(conditionBlock);
    irfunc->getBasicBlockList().push_back(conditionBlock);
    
    builder->SetInsertPoint(conditionBlock);
    builder->CreateCondBr(build(statement->getCondition()), iterationBlock, remergeBlock);
    
    irfunc->getBasicBlockList().push_back(remergeBlock);
    builder->SetInsertPoint(remergeBlock);
    
    table.setBreakBlock(*statement, remergeBlock);
    table.setContinueBlock(*statement, conditionBlock);
}

void codegen::ModuleBuilder::visitPostUntilStmt(ast::PostUntilStmt *statement) {
    llvm::Function *irfunc = builder->GetInsertBlock()->getParent();
    
    llvm::BasicBlock *iterationBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *conditionBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *remergeBlock = llvm::BasicBlock::Create(builder->getContext());
    
    builder->CreateBr(iterationBlock);
    irfunc->getBasicBlockList().push_back(iterationBlock);
    
    builder->SetInsertPoint(iterationBlock);
    if (ast::Stmt *block = statement->getBlock()) {
        build(block);
    }
    builder->CreateTerminatorIfNeeded(conditionBlock);
    irfunc->getBasicBlockList().push_back(conditionBlock);
    
    builder->SetInsertPoint(conditionBlock);
    builder->CreateCondBr(build(statement->getCondition()), remergeBlock, iterationBlock);
    
    irfunc->getBasicBlockList().push_back(remergeBlock);
    builder->SetInsertPoint(remergeBlock);
    
    table.setBreakBlock(*statement, remergeBlock);
    table.setContinueBlock(*statement, conditionBlock);
}

void codegen::ModuleBuilder::visitForStmt(ast::ForStmt *statement) {
    for (ast::Stmt *initStmt : statement->getInitStatements()) build(initStmt);
    
    llvm::Function *irfunc = builder->GetInsertBlock()->getParent();
    
    llvm::BasicBlock *conditionBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *iterationBlock = llvm::BasicBlock::Create(builder->getContext());
    llvm::BasicBlock *remergeBlock = llvm::BasicBlock::Create(builder->getContext());
    
    builder->CreateBr(conditionBlock);
    irfunc->getBasicBlockList().push_back(conditionBlock);
    
    builder->SetInsertPoint(conditionBlock);
    builder->CreateCondBr(build(statement->getCondition()), iterationBlock, remergeBlock);
    irfunc->getBasicBlockList().push_back(iterationBlock);
    
    builder->SetInsertPoint(iterationBlock);
    if (ast::Stmt *block = statement->getBlock()) {
        build(block);
    }
    
    llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(builder->getContext());
    
    builder->CreateTerminatorIfNeeded(endBlock);
    irfunc->getBasicBlockList().push_back(endBlock);
    builder->SetInsertPoint(endBlock);
    for (ast::Stmt *endStmt : statement->getEndStatements()) build(endStmt);
    builder->CreateTerminatorIfNeeded(conditionBlock);
    
    irfunc->getBasicBlockList().push_back(remergeBlock);
    builder->SetInsertPoint(remergeBlock);
    
    table.setBreakBlock(*statement, remergeBlock);
    table.setContinueBlock(*statement, endBlock);
}
