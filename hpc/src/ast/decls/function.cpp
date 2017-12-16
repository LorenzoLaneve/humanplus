// => src/ast/decls/function.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/decls/function.h>
#include <hpc/ast/decls/namespace.h>
#include <hpc/utils/printers.h>
#include <hpc/utils/strings.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/analyzers/validator/validator.h>
#include <hpc/ir/modules.h>
#include <hpc/target/target.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>

#include <iostream>
#include <sstream>

using namespace hpc;

void ast::FunctionDecl::setStatementsBlock(CompoundStmt *stg) {
    this->statements = stg;
    if (stg) if (src::SrcLoc *endref = stg->tokenRef(PointToEndOfCompoundStatement))
        tokenRef(PointToEndOfFunction, *endref);
    localdecls.clear();
}

void ast::FunctionDecl::addLocalDeclaration(ast::Var *variable) {
    localdecls.push_back(variable);
}

int ast::FunctionDecl::containedReturns() {
    if (retcount < 0) {
        retcount = statements->containedReturns();
        if (returnType->isVoidType()) retcount++;
    }
    return retcount;
}

bool ast::FunctionDecl::isMainFunction() const {
    return getName() == __human_plus_main_function_identifier && (!getContainer() || !getContainer()->getContainer());
}

bool ast::FunctionDecl::isNostalgic() const {
    return fattrs.nostalgic;
}

