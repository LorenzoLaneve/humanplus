// => src/analyzers/validator/validator.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/analyzers/validator/resolver.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/decls/namespace.h>

using namespace hpc;

validator::ValidatorInstance::ValidatorInstance(diag::DiagEngine &diags) : diags(diags) {  }

bool validator::ValidatorInstance::validate(ast::AbstractSyntaxTree *ast) {
    assert(ast && "No AST has been passed to the validation session.");

    diag::DiagnosticsReport validationReport;
    diags.addReport(validationReport);
    
    ValidatorImpl visitor(*this, ast);
    visitor.visitAST(*ast);
    
    diags.printAndRemoveReport(validationReport);
    
    return visitor.validationPassed();
}

validator::ValidatorImpl::ValidatorImpl(ValidatorInstance &validator, ast::AbstractSyntaxTree *ast)
: validator(validator), diags(validator.getDiags()), ast(ast) {  }
