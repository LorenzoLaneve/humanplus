// => src/ast/exprs/castings.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/types/type.h>

#include <iostream>

using namespace hpc;

ast::Type *ast::ImplicitCastExpr::evalType() {
    return destination;
}

ast::Type *ast::EvalExpr::evalType() {
    return BuiltinType::get(BuiltinType::Boolean);
}
