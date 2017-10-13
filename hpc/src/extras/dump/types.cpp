// => src/extras/dump/types.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
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

