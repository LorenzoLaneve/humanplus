//
//  types.cpp
//  HumanPlus
//
//  Created by Lore on 09/06/16.
//  Copyright © 2016 Lore. All rights reserved.
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
