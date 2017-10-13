// => src/ast/exprs/members.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/exprs/members.h>
#include <hpc/ast/types/compoundtype.h>
#include <hpc/ast/decls/class.h>
#include <hpc/diagnostics/diagnostics.h>

using namespace hpc;

ast::Type *ast::FieldRef::evalType() {
    return declaration->getType();
}
