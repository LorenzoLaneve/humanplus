// => src/analyzers/validator/resolver.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/resolver.h>
#include <hpc/analyzers/validator/validator.h>
#include <hpc/diagnostics/diagnostics.h>

using namespace hpc;

validator::SymbolResolver::SymbolResolver(validator::ValidatorInstance &validator, ast::AbstractSyntaxTree &ast)
: validator(validator), ast(ast), currentNameSpace(ast.getRootNameSpace()) {  }

void validator::SymbolResolver::switchTo(ast::NameSpaceDecl *nameSpace) {
    assert(nameSpace && "Cannot switch to a null namespace.");
    
    currentNameSpace = nameSpace;
}

void validator::SymbolResolver::switchToContainer() {
    if (ast::NameSpaceDecl *L = currentNameSpace->getContainer()) {
        currentNameSpace = L;
    } else {
        currentNameSpace = ast.getRootNameSpace();
    }
}

void validator::SymbolResolver::openLocalStackForFunction(ast::FunctionDecl *F) {
    localStacks.push_back(LocalStack(*this, F));
}

void validator::SymbolResolver::closeLastLocalStack() {
    localStacks.pop_back();
}

bool validator::SymbolResolver::declareVariable(ast::Var &variable) {
    assert(!localStacks.empty() && "No local stack opened.");
    
    ast::Symbol varSymbol = { variable.getName(), variable.tokenRef(ast::PointToVariableIdentifier) };
    
    if (ast::Var *redefVar = getMatchingVariableNoReport(varSymbol, lookup::Local)) {
        
        diag::DiagEngine &diags = validator.getDiags();
        
        if (ast::Type::areEquivalent(redefVar->getType(), variable.getType()))
            diags.reportError(diag::RedefinitionOfLocalVariable, variable.tokenRef(ast::PointToVariableIdentifier)) << variable.getName();
        else {
            diags.reportError(diag::RedefinitionOfLocalVarWithDifType, variable.tokenRef(ast::PointToVariableIdentifier))
                << variable.getName() << variable.getType()->asString() << redefVar->getType()->asString();
        }
        
        diags.reportNote(diag::PreviousVariableDeclarationWasHere, redefVar->tokenRef(ast::PointToVariableIdentifier)) << redefVar->getName();
        
        return false;
    }
    
    getInnermostStack().addVariable(&variable);
    return true;
}

ast::Var *validator::SymbolResolver::getMatchingLocalVariable(ast::Symbol sympath) {
    if (!sympath.isNested()) {
        ast::SymbolIdentifier topid = sympath.getTopIdentifier();
        
        for (const LocalStack::Scope &scope : getInnermostStack().getLocalScopes()) {
            LocalStack::Scope::const_iterator it = scope.find(topid.identifier);
            
            if (it != scope.end()) return it->second;
        }
    } // FIXME missing local variable refered for lambda closures.
    
    return nullptr;
}

ast::GlobalVar *validator::SymbolResolver::getMatchingGlobalVariable(ast::Symbol sympath, bool noReport) {
    if (sympath.isValid()) {
        ast::NameSpaceDecl *lib = getInnermostMatchingNameSpace(sympath);
        
        if (!lib) {
            if (!noReport) {
                ast::SymbolIdentifier rootID = sympath.getRootIdentifier();
                
                validator.getDiags().reportError(diag::UndeclaredIdentifier, rootID.symref) << rootID.identifier;
            }
            return nullptr;
        }
        
        while (sympath.isNested()) {
            ast::SymbolIdentifier rootid = sympath.getRootIdentifier();
            if (ast::NameSpaceDecl *nlib = lib->getInnerNameSpace(rootid)) lib = nlib;
            else {
                if (!noReport) {
                    validator.getDiags().reportError(diag::NoMemberInNameSpace, rootid.symref) << rootid.identifier << lib->getName();
                }
                return nullptr;
            }
            
            sympath = sympath.containedSymbol();
        }
        
        ast::SymbolIdentifier topID = sympath.getTopIdentifier();
        if (ast::GlobalVar *globalVar = lib->getGlobalVariable(topID)) {
            return globalVar;
        } else if (!noReport) {
            validator.getDiags().reportError(diag::NoMemberInNameSpace, topID.symref) << topID.identifier << lib->getName();
        }
    }
    return nullptr;
}

ast::Var *validator::SymbolResolver::getMatchingVariable(ast::Symbol sympath, int params, bool noReport) {
    if (sympath.isNested()) {
        if (params & lookup::Global) {
            if (ast::GlobalVar *globalVar = getMatchingGlobalVariable(sympath)) {
                return globalVar;
            }
        }
    } else {
        if (params & lookup::Local) {
            if (ast::Var *localVar = getMatchingLocalVariable(sympath)) {
                return localVar;
            }
        }
        
        if (params & lookup::Global) {
            // diags should not be reported in this case, there's the error report after this.
            if (ast::GlobalVar *globalVar = getMatchingGlobalVariableNoReport(sympath)) {
                return globalVar;
            }
        }
        
        if (!noReport) {
            ast::SymbolIdentifier topID = sympath.getTopIdentifier();
            
            validator.getDiags().reportError(diag::UndeclaredIdentifier, topID.symref) << topID.identifier;
        }
    }
    
    return nullptr;
}

ast::NameSpaceDecl *validator::SymbolResolver::getInnermostMatchingNameSpace(ast::Symbol sympath) {
    ast::NameSpaceDecl *lib = currentNameSpace;
    
    if (sympath.isNested()) {
        while (lib && !lib->getInnerNameSpace(sympath.getRootIdentifier().identifier)) {
            lib = lib->getContainer();
        }
    }
    return lib;
}

bool validator::SymbolResolver::getMatchingCandidateFunctions(CandidateFunctions &candidates, ast::Symbol sympath, bool noReport) {
    candidates.clear();
    
    if (sympath.isValid()) {
        ast::NameSpaceDecl *lib = getInnermostMatchingNameSpace(sympath);
        
        if (!lib) {
            if (!noReport) {
                ast::SymbolIdentifier rootID = sympath.getRootIdentifier();
                
                validator.getDiags().reportError(diag::UndeclaredIdentifier, rootID.symref) << rootID.identifier;
            }
            return false;
        }
        
        while (sympath.isNested()) {
            ast::SymbolIdentifier rootid = sympath.getRootIdentifier();
            if (ast::NameSpaceDecl *nlib = lib->getInnerNameSpace(rootid)) lib = nlib;
            else {
                if (!noReport) {
                    validator.getDiags().reportError(diag::NoMemberInNameSpace, rootid.symref) << rootid.identifier << lib->getName();
                }
                return false;
            }
            
            sympath = sympath.containedSymbol();
        }
        
        ast::SymbolIdentifier topID = sympath.getTopIdentifier();
        
        if (const ast::OverloadList *overloads = lib->getOverloads(topID)) {
            for (ast::FunctionDecl *overload : *overloads) {
                candidates.push_back(overload);
            }
        }
        
        // FIXME if no candidates are found, the control should check the container namespace.
        
        if (!candidates.size() && !noReport) {
            ast::SymbolIdentifier topID = sympath.getTopIdentifier();
            validator.getDiags().reportError(diag::UndeclaredIdentifier, topID.symref) << topID.identifier;
            return false;
        }
    }

    return true;
}

ast::Type *validator::SymbolResolver::getMatchingType(ast::Symbol sympath, bool noReport) {
    if (sympath.isValid()) {
        ast::NameSpaceDecl *lib = getInnermostMatchingNameSpace(sympath);
        
        if (!lib) {
            if (!noReport) {
                ast::SymbolIdentifier rootID = sympath.getRootIdentifier();
                validator.getDiags().reportError(diag::UndeclaredIdentifier, rootID.symref) << rootID.identifier;
            }
            return nullptr;
        }
        
        while (sympath.isNested()) {
            ast::SymbolIdentifier rootID = sympath.getRootIdentifier();
            if (ast::NameSpaceDecl *nlib = lib->getInnerNameSpace(rootID)) lib = nlib;
            else {
                if (!noReport) {
                    validator.getDiags().reportError(diag::NoMemberInNameSpace, rootID.symref) << rootID.identifier << lib->getName();
                }
                return nullptr;
            }
            
            sympath = sympath.containedSymbol();
        }
        
        ast::SymbolIdentifier topID = sympath.getTopIdentifier();
        if (ast::Type *typeRef = lib->getType(topID)) {
            return typeRef;
        } else if (!noReport) {
            if (!lib->getName().empty())
                validator.getDiags().reportError(diag::NoTypeInNameSpace, topID.symref) << topID.identifier << lib->getName();
            else
                validator.getDiags().reportError(diag::UndeclaredIdentifier, topID.symref) << topID.identifier;
        }
        
    }
    
    return nullptr;
}


validator::LocalStack::LocalStack(validator::SymbolResolver &resolver, ast::FunctionDecl *F) : resolver(resolver), F(F) {  }

void validator::LocalStack::addVariable(ast::Var *variable) {
    assert(!localStack.empty() && "No scope ready to take the incoming variable.");
    
    Scope &theScope = localStack.back();
    
    theScope[variable->getName()] = variable;
}

void validator::LocalStack::clear() {
    localStack.clear();
    
    breakCatchers.clear();
    continueCatchers.clear();
    caseCatchers.clear();
}

void validator::LocalStack::pushBreakCatcher(ast::Stmt *responder) {
    breakCatchers.push_back(responder);
}

void validator::LocalStack::pushContinueCatcher(ast::Stmt *responder) {
    continueCatchers.push_back(responder);
}

void validator::LocalStack::pushCaseCatcher(ast::Stmt *responder) {
    caseCatchers.push_back(responder);
}

void validator::LocalStack::popBreakCatcher() {
    breakCatchers.pop_back();
}

void validator::LocalStack::popContinueCatcher() {
    continueCatchers.pop_back();
}

void validator::LocalStack::popCaseCatcher() {
    caseCatchers.pop_back();
}

ast::Stmt *validator::LocalStack::getFirstBreakCatcher(ast::break_target target) {
    unsigned long i = breakCatchers.size();
    
    while (i--) if (breakCatchers[i]->getBreakRole() & target) return breakCatchers[i];
    return nullptr;
}

ast::Stmt *validator::LocalStack::getFirstContinueCatcher(ast::continue_target target) {
    unsigned long i = continueCatchers.size();
    
    while (i--) if (continueCatchers[i]->getContinueRole() & target) return continueCatchers[i];
    return nullptr;
}

ast::Stmt *validator::LocalStack::getFirstCaseCatcher() {
    if (caseCatchers.empty()) return nullptr;
    return caseCatchers.back();
}
