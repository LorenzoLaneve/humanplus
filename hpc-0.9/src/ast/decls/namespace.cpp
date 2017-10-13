//
//  NameSpaceDecl.cpp
//  Human Plus
//
//  Created by Lore on 08/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/ast/decls/namespace.h>
#include <hpc/ast/decls/typealias.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/decls/function.h>
#include <hpc/ast/decls/class.h>
#include <hpc/ast/decls/protocol.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/utils/printers.h>
#include <hpc/utils/strings.h>
#include <hpc/analyzers/validator/validator.h>

using namespace hpc;

void ast::NameSpaceDecl::addInnerNameSpace(ast::NameSpaceDecl *ns) {
    declarations.push_back(ns);
    namespaces[ns->getName()] = ns;
    ns->setContainer(this);
}

void ast::NameSpaceDecl::addTypeAlias(ast::TypeAliasDecl *tpa) {
    declarations.push_back(tpa);
    typeAliases.push_back(tpa);
    addType(tpa->getName(), tpa->getType());
}

void ast::NameSpaceDecl::addGlobalVariable(ast::GlobalVar *gvr) {
    declarations.push_back(gvr);
    if (!globalVars[gvr->getName()]) globalVars[gvr->getName()] = gvr;
}

void ast::NameSpaceDecl::addFunction(ast::FunctionDecl *fnc) {
    declarations.push_back(fnc);
    functions[fnc->getName()].push_back(fnc);
    fnc->setContainer(this);
}

void ast::NameSpaceDecl::addClass(ast::ClassDecl *cls) {
    addInnerNameSpace(cls);
    classes.push_back(cls);
    if (!types[cls->getName()]) types[cls->getName()] = cls->getType();
}

void ast::NameSpaceDecl::addProtocol(ast::ProtocolDecl *ptc) {
    declarations.push_back(ptc);
    protocols.push_back(ptc);
}

void ast::NameSpaceDecl::addType(std::string identifier, ast::Type *type) {
    if (!types[identifier]) types[identifier] = type;
}

ast::NameSpaceDecl *ast::NameSpaceDecl::getInnerNameSpace(ast::Symbol sympath) {
    if (sympath.isValid()) {
        namespace_table::const_iterator theLib = namespaces.find(sympath.getRootIdentifier().identifier);
        
        if (theLib != namespaces.end()) {
            if (sympath.isNested()) {
                return (theLib->second)->getInnerNameSpace(sympath.containedSymbol());
            } else return theLib->second;
        } else return nullptr;
    }
    
    return this;
}

ast::GlobalVar *ast::NameSpaceDecl::getGlobalVariable(ast::Symbol sympath) {
    if (sympath.isValid()) {
        ast::NameSpaceDecl *theLib = this;
        if (sympath.isNested()) {
            theLib = getInnerNameSpace(sympath.containerSymbol());
        }
        
        var_table::const_iterator theVar = globalVars.find(sympath.getTopIdentifier().identifier);
        
        if (theVar != globalVars.end()) {
            return theVar->second;
        }
    }
    
    return nullptr;
}

const ast::OverloadList *ast::NameSpaceDecl::getOverloads(ast::Symbol sympath) {
    if (sympath.isValid()) {
        ast::NameSpaceDecl *theLib = this;
        if (sympath.isNested()) {
            theLib = getInnerNameSpace(sympath.containerSymbol());
        }
        
        func_table::const_iterator theList = functions.find(sympath.getTopIdentifier().identifier);
        
        if (theList != functions.end()) {
            return &theList->second;
        }
    }
    
    return nullptr;
}

ast::Type *ast::NameSpaceDecl::getType(ast::Symbol sympath) {
    if (sympath.isValid()) {
        ast::NameSpaceDecl *theLib = this;
        if (sympath.isNested()) {
            theLib = getInnerNameSpace(sympath.containerSymbol());
        }
        
        type_table::const_iterator theType = types.find(sympath.getTopIdentifier().identifier);
        
        if (theType != types.end()) {
            return theType->second;
        }
    }
    
    return nullptr;
}

