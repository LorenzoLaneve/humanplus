//
//  toplevel.cpp
//  HumanPlus
//
//  Created by Lore on 11/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ir/builders.h>
#include <hpc/target/target.h>
#include <hpc/ast/decls/namespace.h>

#include <hpc/ast/types/builtintype.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/decls/function.h>

#include <llvm/IR/Value.h>

using namespace hpc;

void codegen::ModuleBuilder::visitNameSpaceDecl(ast::NameSpaceDecl *nameSpace) {
    
    for (ast::Decl *decl : nameSpace->getDeclarations())
        takeDecl(decl);
    
}

void codegen::ModuleBuilder::visitGlobalVar(ast::GlobalVar *var) {
    llvm::Module &module = getModule();
    
    llvm::Value *initializer = nullptr;
    llvm::BasicBlock *ginitblock = nullptr;
    
    llvm::Constant *constinit = nullptr;
    if (var->getInitialValue()) {
        builder = new InstructionBuilder(module.getContext());
        ginitblock = llvm::BasicBlock::Create(module.getContext());
        builder->SetInsertPoint(ginitblock);
        
        
        initializer = build(var->getInitialValue());
        
        constinit = llvm::dyn_cast<llvm::Constant>(initializer);
    }
    
    bool isConstantInit = constinit;
    if (!constinit) {
        constinit = llvm::Constant::getNullValue(getIRType(var->getType()));
    }
    
    llvm::GlobalVariable *GV = new llvm::GlobalVariable(module, getIRType(var->getType()), /*var.getType()->isConstant()*/false,
                                                        llvm::GlobalValue::ExternalLinkage, constinit, getTargetInfo().getMangle().mangleGlobalVariable(var));
    //GV->setAlignment(type->getAlignment());
    table.setValForComponent(var, GV);
    
    if (var->getInitialValue() && !isConstantInit) {
        llvm::Function *globalinit = llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getVoidTy(builder->getContext()), false),
                                                            llvm::GlobalValue::InternalLinkage, "__hplus_global_var_init", &module);
        globalinit->setSection("__TEXT,__StaticInit,regular,pure_instructions");
        globalinit->addFnAttr(llvm::Attribute::StackProtect);
        globalinit->addFnAttr(llvm::Attribute::UWTable);
        assign(var, initializer);
        builder->CreateRetVoid();
        globalinit->getBasicBlockList().push_back(ginitblock);
    }
    builder = nullptr;
}
