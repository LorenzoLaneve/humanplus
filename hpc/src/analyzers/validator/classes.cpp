// => src/analyzers/validator/classes.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/decls/class.h>
#include <hpc/ast/decls/protocol.h>

using namespace hpc;

void validator::ValidatorImpl::visitClassDecl(ast::ClassDecl *classDecl) {
    visitNameSpaceDecl(classDecl);
    
    getResolver().switchTo(classDecl);
    for (ast::Decl *member : classDecl->getMembers()) {
        if (!validate(member)) {
            classDecl->resignValidation();
        }
    }
    getResolver().switchToContainer();
    
    if (ast::Type *redefTy = getResolver().getMatchingType(ast::Symbol(classDecl->getName()))) {
        if (classDecl->getType() != redefTy) {
            validator.getDiags().reportError(diag::RedefinitionOfType, classDecl->tokenRef(ast::PointToVariableIdentifier));
            
            validator.getDiags().reportNote(diag::PreviousTypeDeclarationWasHere, redefTy->tokenRef(ast::PointToVariableIdentifier))
                << classDecl->getName();
            
            classDecl->resignValidation();
        }
    }
}

void validator::ValidatorImpl::visitProtocolDecl(ast::ProtocolDecl *protocolDecl) {
    // TODO
}
