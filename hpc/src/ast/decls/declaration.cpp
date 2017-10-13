// => src/ast/decls/declaration.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
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
