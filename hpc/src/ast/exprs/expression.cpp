// => src/ast/exprs/expression.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/exprs/expression.h>
#include <hpc/analyzers/sources.h>

using namespace hpc;

source::SrcLoc *ast::Expr::completeRef() {
    return source::SrcLoc::join(tokenRef(PointToBeginOfExpression), tokenRef(PointToEndOfExpression));
}



