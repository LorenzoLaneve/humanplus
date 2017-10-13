//
//  members.cpp
//  HumanPlus
//
//  Created by Lore on 01/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ast/exprs/members.h>
#include <hpc/ast/types/compoundtype.h>
#include <hpc/ast/decls/class.h>
#include <hpc/diagnostics/diagnostics.h>

using namespace hpc;

ast::Type *ast::FieldRef::evalType() {
    return declaration->getType();
}
