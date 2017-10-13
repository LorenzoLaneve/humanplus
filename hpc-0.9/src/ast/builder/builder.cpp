//
//  builder.cpp
//  HumanPlus
//
//  Created by Lore on 14/03/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#include <hpc/ast/builder/builder.h>

using namespace hpc;

ast::Builder::Builder(ast::AbstractSyntaxTree *tree) : tree(tree) {
    insertNameSpace = tree->getRootNameSpace();
}

ast::CompilationUnit *ast::Builder::getOrCreateUnit(fsys::InputFile *ifile) {
    if (ast::CompilationUnit *theUnit = tree->getUnitForFile(ifile)) {
        return theUnit;
    }

    ast::CompilationUnit *newUnit = new CompilationUnit(ifile);
    tree->addUnit(newUnit);
    
    return newUnit;
}

ast::NameSpaceDecl *ast::Builder::getOrCreateNameSpace(std::string name) {
    ast::NameSpaceDecl &superNameSpace = getInsertNameSpace();
    
    if (ast::NameSpaceDecl *lib = superNameSpace.getInnerNameSpace(name)) {
        return lib;
    } else {
        ast::NameSpaceDecl *newNameSpace = new ast::NameSpaceDecl(name);
        superNameSpace.addInnerNameSpace(newNameSpace);
        return newNameSpace;
    }
}

ast::GlobalVar *ast::Builder::createGlobalVariable(std::string varName, Type *type, Expr *initialValue) {
    ast::NameSpaceDecl &lib = getInsertNameSpace();
    
    ast::GlobalVar *newVar = new ast::GlobalVar(varName, type, &lib, initialValue);
    getInsertUnit().addTopLevelDecl(newVar);
    
    lib.addGlobalVariable(newVar);
    return newVar;
}

ast::FunctionDecl *ast::Builder::createFunctionDecl(std::string functionName, std::vector<ParamVar *> arguments, Type *returnType,
                                                    FunctionDecl::FunctionAttributes attrs) {
    ast::NameSpaceDecl &lib = getInsertNameSpace();
    if (attrs.nostalgic) {
        lib = *tree->getRootNameSpace();
    }
    
    ast::FunctionDecl *newDecl = new ast::FunctionDecl(functionName, arguments, returnType, attrs);
    getInsertUnit().addTopLevelDecl(newDecl);
    // FIXME even if nostalgic, functions should have the visibility of the nameSpace they're declared in.
    
    lib.addFunction(newDecl);
    return newDecl;
}

ast::TypeAliasDecl *ast::Builder::createTypeAliasDecl(std::string aliasName, Type *originalType) {
    ast::NameSpaceDecl &lib = getInsertNameSpace();
    
    ast::TypeAliasDecl *newDecl = new ast::TypeAliasDecl(aliasName, originalType, &lib);
    getInsertUnit().addTopLevelDecl(newDecl);
    
    lib.addTypeAlias(newDecl);
    return newDecl;
}

ast::ClassDecl *ast::Builder::createClassDecl(std::string className, Symbol *superClass, std::vector<Symbol *> protocols) {
    ast::NameSpaceDecl &lib = getInsertNameSpace();
    
    ast::ClassDecl *newDecl = new ast::ClassDecl(className);
    getInsertUnit().addTopLevelDecl(newDecl);
    
    lib.addClass(newDecl);
    return newDecl;
}
