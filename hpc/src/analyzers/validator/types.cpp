// => src/analyzers/validator/types.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>

#include <hpc/ast/types/base.h>
#include <hpc/ast/types/builtintype.h>
#include <hpc/ast/types/pointertype.h>
#include <hpc/ast/types/compoundtype.h>

using namespace hpc;

void validator::ValidatorImpl::visitPointerType(ast::PointerType *type) {
    
    if (!validate(type->getPointedType())) {
        type->resignValidation();
    }
    
}

void validator::ValidatorImpl::visitClassType(ast::ClassType *type) {
    // TODO
}
