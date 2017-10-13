//
//  declaration.cpp
//  HumanPlus
//
//  Created by Lore on 11/05/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ast/decls/declaration.h>
#include <hpc/ast/decls/namespace.h>

using namespace hpc;

ast::Symbol ast::GlobalDecl::getSymbolPath() const {
    ast::Symbol theSymbol = getName();
    
    ast::NameSpaceDecl *nameSpace = getContainer();
    while (nameSpace && nameSpace->getContainer()) {
        theSymbol.pushFrontChild(nameSpace->getName());
        
        nameSpace = nameSpace->getContainer();
    }
    
    return theSymbol;
}
