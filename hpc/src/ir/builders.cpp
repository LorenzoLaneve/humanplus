// => src/ir/builders.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ir/builders.h>
#include <hpc/ir/modules.h>
#include <hpc/ast/types/base.h>
#include <hpc/ast/types/builtintype.h>
#include <hpc/ast/types/pointertype.h>
#include <hpc/ast/types/compoundtype.h>
#include <hpc/ast/decls/class.h>
#include <hpc/ast/exprs/reference.h>
#include <hpc/ast/exprs/members.h>
#include <hpc/target/target.h>


#include <llvm/Support/Casting.h>

using namespace hpc;

codegen::SymbolTable::SymbolTable(ModuleBuilder &moduleBuilder) : moduleBuilder(moduleBuilder) {  }

void codegen::SymbolTable::setValForComponent(ast::Component *component, llvm::Value *value) {
    componentTable[component] = value;
}

void codegen::SymbolTable::setValForComponent(ast::Component &component, llvm::Value *value) {
    componentTable[&component] = value;
}

llvm::Value *codegen::SymbolTable::getValForComponent(ast::Component *component) {
    return componentTable[component];
}

llvm::Value *codegen::SymbolTable::getValForComponent(ast::Component &component) {
    return componentTable[&component];
}

void codegen::SymbolTable::setIRType(ast::Type *type, llvm::Type *irtype) {
    typeTable[type] = irtype;
}

void codegen::SymbolTable::setIRType(ast::Type &type, llvm::Type *irtype) {
    typeTable[&type] = irtype;
}

llvm::Type *codegen::SymbolTable::getIRType(ast::Type *type) {
    if (!typeTable[type]) {
        moduleBuilder.takeType(type);
    }
    return typeTable[type];
}

llvm::Type *codegen::SymbolTable::getIRType(ast::Type &type) {
    return getIRType(&type);
}

llvm::Value *codegen::SymbolTable::getOrCreateReference(ast::Component *component) {
    if (ast::Var *var = llvm::dyn_cast<ast::Var>(component)) {
        return getValForComponent(var);
    }
    
    if (ast::VarRef *varRef = llvm::dyn_cast<ast::VarRef>(component)) {
        return getValForComponent(varRef->getVar());
    }
    
    if (ast::FieldRef *fieldRef = llvm::dyn_cast<ast::FieldRef>(component)) {
        return moduleBuilder.getInstBuilder().CreateStructGEP(getIRType(fieldRef->getDeclaration()->getEnclosingType()),
                                                              getOrCreateReference(fieldRef->getEntity()),
                                                              fieldRef->getDeclaration()->getFieldIndex()
                                                              );
    }
    
    return nullptr;
}

llvm::Value *codegen::SymbolTable::getOrCreateReference(ast::Component &component) {
    return getOrCreateReference(&component);
}

void codegen::SymbolTable::setBreakBlock(ast::Stmt &stmt, llvm::BasicBlock *block) {
    breakTable[&stmt] = block;
}

void codegen::SymbolTable::setContinueBlock(ast::Stmt &stmt, llvm::BasicBlock *block) {
    continueTable[&stmt] = block;
}

llvm::GlobalVariable *codegen::SymbolTable::getOrCreateStringConstant(runtime::string_ty str) {
    if (!stringTable[str]) {
        
        llvm::Module &module = moduleBuilder.getModule();
        
        ast::Type *stringType = ast::BuiltinType::get(ast::BuiltinType::Character)->pointerTo();
        ast::Type *charType = stringType->getPointedType();

        llvm::ArrayType *arrayType = llvm::ArrayType::get(getIRType(charType), str.length()+1);
        
        llvm::GlobalVariable *stringConstant = new llvm::GlobalVariable(
                                                                        module,
                                                                        arrayType,
                                                                        /*isConstant=*/true,
                                                                        llvm::GlobalValue::PrivateLinkage,
                                                                        llvm::ConstantDataArray::getString(module.getContext(), str.c_str()),
                                                                        ".str"
                                                                        );
        
        stringConstant->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Local); // FIXME alignment
        //strcst->setAlignment(chrtype->getAlignment());

        stringTable[str] = stringConstant;
        
    }
    return stringTable[str];
}

llvm::BranchInst *codegen::SymbolTable::makeBreak(ast::Stmt *stmt) {
    return moduleBuilder.getInstBuilder().CreateBr(breakTable[stmt]);
}

llvm::BranchInst *codegen::SymbolTable::makeContinue(ast::Stmt *stmt) {
    return moduleBuilder.getInstBuilder().CreateBr(continueTable[stmt]);
}


codegen::ModuleBuilder::ModuleBuilder(modules::ModuleWrapper &moduleWrapper, target::TargetInfo &targetInfo)
    : moduleWrapper(moduleWrapper), targetInfo(targetInfo), table(*this) {  }
