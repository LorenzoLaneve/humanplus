// => src/analyzers/validator/exprs/resolution.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/symbols.h>
#include <hpc/ast/types/base.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/decls/function.h>
#include <hpc/ast/decls/class.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/exprs/reference.h>
#include <hpc/ast/exprs/members.h>

using namespace hpc;

static int getTypeAffinity(ast::FunctionDecl *candidate, const std::vector<ast::Expr *> &actualParams) {
    int i = 0, typeAffinity = 0;
    for (ast::ParamVar *argument : candidate->getArgs()) {
        ast::Type *actualType = actualParams[i++]->evalType(); // FIXME don't use indexes.
        
        if (ast::Type::areEquivalent(actualType, argument->getType())) {
            typeAffinity += 2;
        } else if (actualType->canAssignTo(argument->getType())) {
            typeAffinity += 1;
        } else {
            return -1;
        }
    }
    return typeAffinity;
}

void validator::ValidatorImpl::visitTypeRef(ast::TypeRef *typeRef) { // FIXME attribs checks
    if (typeRef->getType()) {
        return;
    }
    
    if (ast::Type *theType = getResolver().getMatchingType(typeRef->getSymbolPath())) {
        if (validate(theType)) {
            typeRef->setType(theType);
        }
    }
    
    if (!typeRef->getType()) {
        typeRef->resignValidation();
        typeRef->setPlaceholderType();
        return;
    }
    
    if (!validate(typeRef->getType())) typeRef->resignValidation();
}

void validator::ValidatorImpl::visitVarRef(ast::VarRef *varRef) {
    if (ast::Var *theVar = getResolver().getMatchingVariable(varRef->getSymbol())) {
        varRef->setVar(theVar);
        
        if (!theVar->isValid()) varRef->resignValidation();
    } else {
        varRef->resignValidation();
    }
}

void validator::ValidatorImpl::visitFunctionCall(ast::FunctionCall *functionCall) {
    
    const std::vector<ast::Expr *> &actualParams = functionCall->getActualParams();
    
    for (ast::Expr *paramVal : actualParams) {
        if (!validate(paramVal)) functionCall->resignValidation();
    }
    
    if (!functionCall->isValid()) return;
    
    CandidateFunctions candidateFunctions, functionOverloads;
    
    int maxTypeAffinity = 0;
    if (getResolver().getMatchingCandidateFunctions(functionOverloads, functionCall->getSymbol())) {
        
        for (ast::FunctionDecl *candidate : functionOverloads) if (actualParams.size() == candidate->getArgs().size()) {
            int typeAffinity = getTypeAffinity(candidate, actualParams);
            
            if (typeAffinity >= maxTypeAffinity) {
                if (typeAffinity > maxTypeAffinity) { // this candidate function has a better type affinity than the previous ones, remove them.
                    candidateFunctions.clear();
                    maxTypeAffinity = typeAffinity;
                }
                candidateFunctions.push_back(candidate);
            }
        }
        
        ast::SymbolIdentifier topID = functionCall->getSymbol().getTopIdentifier();
        if (!candidateFunctions.empty()) {
            if (candidateFunctions.size() > 1) {
                validator.getDiags().reportError(diag::FunctionCallIsAmbiguous, topID.symref) << topID.identifier;
                
                for (ast::FunctionDecl *candidate : candidateFunctions)
                    validator.getDiags().reportNote(diag::CandidateFunction, candidate->tokenRef(ast::PointToVariableIdentifier));
                
                functionCall->resignValidation();
            }
            
            int i = 0;
            ast::FunctionDecl *thePrototype = candidateFunctions.front();
            for (ast::ParamVar *argument : thePrototype->getArgs()) {
                functionCall->castActualParamToType(i++, argument->getType());
            }
            functionCall->setFunctionDecl(thePrototype);
        } else {
            validator.getDiags().reportError(diag::FunctionOverloadDoesNotExist, topID.symref) << topID.identifier;
            functionCall->resignValidation();
        }
    } else {
        functionCall->resignValidation();
    }
}

void validator::ValidatorImpl::visitFieldRef(ast::FieldRef *fieldRef) {
    if (!validate(fieldRef->getEntity())) {
        fieldRef->resignValidation();
        return;
    }
    
    ast::Type *entityTy = fieldRef->getEntity()->evalType();
    
    ast::FieldDecl *theField = entityTy->getMember(fieldRef->getMemberIdentifier());
    
    if (!theField) {
        validator.getDiags().reportError(diag::NoObjectMemberInType, fieldRef->tokenRef(ast::PointToStructAccessOperator))
            << fieldRef->getMemberIdentifier() << entityTy->asString();
        fieldRef->resignValidation();
        return;
    }
    
    fieldRef->setDeclaration(theField);
    if (!theField->isValid()) fieldRef->resignValidation();
}
