// => src/ir/functions.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ir/builders.h>
#include <hpc/ast/decls/function.h>
#include <hpc/ast/types/builtintype.h>

#include <llvm/IR/Value.h>

using namespace hpc;

void codegen::ModuleBuilder::visitFunctionDecl(ast::FunctionDecl *function) {
    if (!table.getValForComponent(function)) {
        llvm::Module &module = getModule();
        
        const std::vector<ast::ParamVar *> &arguments = function->getArgs();
        std::vector<llvm::Type *> argtypes;
        
        ast::Type *returnType = function->getReturnType();
        
        for (ast::ParamVar *arg : arguments) {
            llvm::Type *llty = getIRType(arg->getType());
            // TODO zeroext, signext
            argtypes.push_back(llty);
        }
        
        
        llvm::Function *irfunc = llvm::Function::Create(llvm::FunctionType::get(getIRType(returnType), argtypes, false),
                                                        llvm::GlobalValue::ExternalLinkage,
                                                        getTargetInfo().getMangle().mangleFunction(function),
                                                        &module);
        
        unsigned i = 0;
        for (auto &arg : irfunc->args()) {
            ast::Type *paramty = arguments[i++]->getType();
            if (paramty->isIntegerType()) {
                ast::BuiltinType *btype = static_cast<ast::BuiltinType *>(paramty);
                
                if (btype->getMagnitude() < 4) { // 4 is integer magnitude.
                    // FIXME take 4 (4*8 = 32) from the targetting machine minimum integer type supported.
                    irfunc->addAttribute(arg.getArgNo()+1, paramty->isSignedIntegerType() ? llvm::Attribute::SExt : llvm::Attribute::ZExt);
                }
            }
        }
        
        if (ast::CompoundStmt *statementsBlock = function->getStatementsBlock()) {
            i = 0;
            for (auto &arg : irfunc->args()) {
                arg.setName(arguments[i++]->getName());
            }
            
            llvm::BasicBlock *mainBlock = llvm::BasicBlock::Create(module.getContext(), "", irfunc);
            
            builder = new InstructionBuilder(module.getContext());
            builder->SetInsertPoint(mainBlock);
            
            if (function->containedReturns() > 1) {
                returnBlock = llvm::BasicBlock::Create(module.getContext(), "");
                if (!returnType->isVoidType()) {
                    returnRegister = builder->CreateAlloca(getIRType(returnType));
                    //returnRegister->setAlignment(rtype->getType()->getAlignment());
                }
            }
            
            for (ast::ParamVar *arg : arguments) {
                table.setValForComponent(arg, builder->CreateAlloca(getIRType(arg->getType())));
                // FIXME alignment
            }
            
            for (ast::Var *localVar : function->getLocalVars()) {
                table.setValForComponent(localVar, builder->CreateAlloca(getIRType(localVar->getType()), nullptr, localVar->getName()));
                // FIXME alignment
            }
            
            i = 0;
            for (auto &arg : irfunc->args()) assign(arguments[i++], &arg);
            for (ast::Stmt *stmt : statementsBlock->statements()) takeStmt(stmt);
            
            bool nrets = !statementsBlock->returns();
            if (function->containedReturns() > 1) {
                
                if (returnType->isVoidType() && nrets) builder->CreateBr(returnBlock);
                irfunc->getBasicBlockList().push_back(returnBlock);
                builder->SetInsertPoint(returnBlock);
                
                if (!returnType->isVoidType()) {
                    builder->CreateRet(builder->CreateLoad(returnRegister)); // FIXME alignment
                } else {
                    builder->CreateRetVoid();
                }
            } else if (nrets && returnType->isVoidType()) {
                builder->CreateRetVoid();
            }
            
            builder = nullptr;
        }
        
        //if(function.isExceptionSafe())
        irfunc->addFnAttr(llvm::Attribute::NoUnwind);
        irfunc->addFnAttr(llvm::Attribute::StackProtect);
        irfunc->addFnAttr(llvm::Attribute::UWTable);
        
        assert(llvm::verifyFunction(&irfunc, &llvm::errs()) && "Function verification failed.");
        
        table.setValForComponent(function, irfunc);
    }
}


