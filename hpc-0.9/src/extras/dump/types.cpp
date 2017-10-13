//
//  types.cpp
//  HumanPlus
//
//  Created by Lore on 09/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/extras/dump/dump.h>

using namespace hpc;

void extras::NewASTPrinter::visitType(ast::Type *type) {
    os << type->asString();
}

void extras::NewASTPrinter::visitTypeRef(ast::TypeRef *typeRef) {
    if (!typeRef->getType()) {
        os << "invalid type";
        return;
    }
    
    visitType(typeRef);
}

