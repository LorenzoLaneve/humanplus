//
//  expressions.cpp
//  HumanPlus
//
//  Created by Lore on 09/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/extras/dump/dump.h>
#include <hpc/analyzers/syntax/operators.h>

using namespace hpc;

void extras::NewASTPrinter::visitBinaryExpr(ast::BinaryExpr *expression) {
    printObject("BinaryExpr", expression);
    os << "'";
    util::printOperator(expression->getOperator());
    os << "'\n";
    
    openChildBranch(2);
    takeStmt(expression->getLHS());
    takeStmt(expression->getRHS());
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitUnaryExpr(ast::UnaryExpr *expression) {
    printObject("UnaryExpr", expression);
    os << "'";
    util::printOperator(expression->getOperator());
    os << "'\n";
    
    openChildBranch(1);
    takeStmt(expression->getOperand());
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitCharLiteral(ast::CharLiteral *literal) {
    printObject("CharLiteral", literal);
    util::printEscapedChar(literal->getValue());
    os << "'\n";
}

void extras::NewASTPrinter::visitStringLiteral(ast::StringLiteral *literal) {
    printObject("StringLiteral", literal);
    os << "\"";
    util::printEscapedString(literal->getValue());
    os << "\"\n";
}

void extras::NewASTPrinter::visitIntegerLiteral(ast::IntegerLiteral *literal) {
    printObject("IntegerLiteral", literal);
    os << literal->getValue() << "\n";
}

void extras::NewASTPrinter::visitUIntegerLiteral(ast::UIntegerLiteral *literal) {
    printObject("UIntegerLiteral", literal);
    os << literal->getValue() << "\n";
}

void extras::NewASTPrinter::visitLongLiteral(ast::LongLiteral *literal) {
    printObject("LongLiteral", literal);
    os << literal->getValue() << "\n";
}

void extras::NewASTPrinter::visitULongLiteral(ast::ULongLiteral *literal) {
    printObject("ULongLiteral", literal);
    os << literal->getValue() << "\n";
}

void extras::NewASTPrinter::visitFloatLiteral(ast::FloatLiteral *literal) {
    printObject("FloatLiteral", literal);
    os << literal->getValue() << "\n";
}

void extras::NewASTPrinter::visitDoubleLiteral(ast::DoubleLiteral *literal) {
    printObject("DoubleLiteral", literal);
    os << literal->getValue() << "\n";
}

void extras::NewASTPrinter::visitBoolLiteral(ast::BoolLiteral *literal) {
    printObject("BoolLiteral", literal);
    os << (literal->getValue() ? "true" : "false") << "\n";
}

void extras::NewASTPrinter::visitNullPointer(ast::NullPointer *nullpointer) {
    printObject("NullPointer", nullpointer);
    os << "\n";
}

void extras::NewASTPrinter::visitVarRef(ast::VarRef *varRef) {
    printObject("VarRef", varRef);
    os << varRef->getSymbol().str() << "\n";
}

void extras::NewASTPrinter::visitFunctionCall(ast::FunctionCall *functionCall) {
    printObject("FunctionCall", functionCall);
    os << functionCall->getSymbol().str() << "\n";
    
    openChildBranch(functionCall->getActualParams().size());
    for (ast::Expr *actParam : functionCall->getActualParams()) {
        takeStmt(actParam);
    }
    closeLastChildBranch();
}

void extras::NewASTPrinter::visitFieldRef(ast::FieldRef *fieldRef) {
    printObject("FieldRef", fieldRef);
    os << fieldRef->getMemberIdentifier() << "\n";
    
    openChildBranch(1);
    takeStmt(fieldRef->getEntity());
    closeLastChildBranch();
}


void extras::NewASTPrinter::visitGlobalVar(ast::GlobalVar *var) {
    printObject("GlobalVar", var);
    os << var->getName() << " ";
    
    if (ast::Type *type = var->getType()) {
        takeType(type);
    } else os << "type inference failed";
    os << "\n";
    
    if (ast::Expr *initVal = var->getInitialValue()) {
        openChildBranch(1);
        takeStmt(initVal);
        closeLastChildBranch();
    }
}

void extras::NewASTPrinter::visitLocalVar(ast::LocalVar *var) {
    printObject("LocalVar", var);
    os << var->getName() << " ";
    
    if (ast::Type *type = var->getType()) {
        takeType(type);
    } else os << "type inference failed";
    os << "\n";
    
    if (ast::Expr *initVal = var->getInitialValue()) {
        openChildBranch(1);
        takeStmt(initVal);
        closeLastChildBranch();
    }
    
}

void extras::NewASTPrinter::visitParamVar(ast::ParamVar *var) {
    printObject("ParamVar", var);
    os << var->getName() << " ";
    
    if (ast::Type *type = var->getType()) {
        takeType(type);
    } else os << "type inference failed";
    os << "\n";
    
    if (ast::Expr *defaultVal = var->getDefaultValue()) {
        openChildBranch(1);
        takeStmt(defaultVal);
        closeLastChildBranch();
    }
}

void extras::NewASTPrinter::visitFieldDecl(ast::FieldDecl *fieldDecl) {
    printObject("FieldDecl", fieldDecl);
    os << fieldDecl->getName() << " ";
    
    if (ast::Type *type = fieldDecl->getType())
        takeType(type);
    else os << "type inference failed";
    os << "\n";
    
    if (ast::Expr *initVal = fieldDecl->getInitialValue()) {
        openChildBranch(1);
        takeStmt(initVal);
        closeLastChildBranch();
    }
}
