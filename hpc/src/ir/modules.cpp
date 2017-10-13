// => src/ir/modules.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ir/modules.h>
#include <hpc/ir/builders.h>
#include <hpc/target/target.h>
#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/config.h>

using namespace hpc;

modules::ModuleWrapper::ModuleWrapper(std::string modulename, llvm::LLVMContext &context) {
    module = new llvm::Module(modulename, context);
}

llvm::Module &modules::ModuleWrapper::getModule() {
    assert(module && "Module not initialized yet.");
    return *module;
}

void modules::ModuleWrapper::initialize() {
    // Initialize a module before starting to parse its source file
    //module->setDataLayout(target::getDataLayout());
    //module->setTargetTriple(target::getTriple()->getTriple());
}

void modules::ModuleWrapper::finalize() {
    // Add the last things to the module
    //target::addTargetAttribsToFunctions(*module);
    
    module->setPICLevel(llvm::PICLevel::Large);
    
    llvm::NamedMDNode *ident = module->getOrInsertNamedMetadata("llvm.ident");
    ident->addOperand(llvm::MDNode::get(module->getContext(), { llvm::MDString::get(module->getContext(), __human_plus_compiler_signature) }));
}

void modules::ModuleWrapper::dump() {
    module->dump();
}

void modules::ModuleWrapper::addDeclaration(ast::Decl *declaration) {
    declarations.push_back(declaration);
}

