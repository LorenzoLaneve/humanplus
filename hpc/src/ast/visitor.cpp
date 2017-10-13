// => src/ast/visitor.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/visitor.h>

using namespace hpc;

/*
void ast::Visitor::visitType(ast::Type &type) { visitComponent(type); }
void ast::Visitor::visitTypeRef(ast::TypeRef &type) { visitType(type); }
void ast::Visitor::visitAliasedType(ast::AliasedType &type) { visitType(type); }
void ast::Visitor::visitQualifiedType(ast::QualifiedType &type) { visitType(type); }

void ast::Visitor::visitBuiltinType(ast::BuiltinType &type) { visitType(type); }
void ast::Visitor::visitPointerType(ast::PointerType &type) { visitType(type); }
void ast::Visitor::visitClassType(ast::ClassType &type) { visitType(type); }

void ast::Visitor::visitDeclaration(ast::Decl &decl) { visitComponent(decl); }
void ast::Visitor::visitNamedDeclaration(ast::NamedDecl &decl) { visitDeclaration(decl); }
void ast::Visitor::visitGlobalDeclaration(ast::GlobalDecl &decl) { visitNamedDeclaration(decl); }
void ast::Visitor::visitNameSpace(ast::NameSpaceDecl &nameSpace) { visitDeclaration(nameSpace); }

void ast::Visitor::visitVar(ast::Var &var) { visitDeclaration(var); }
void ast::Visitor::visitGlobalVar(ast::GlobalVar &var) { visitVar(var); }
void ast::Visitor::visitLocalVar(ast::LocalVar &var) { visitVar(var); }
void ast::Visitor::visitParamVar(ast::ParamVar &var) { visitVar(var); }

void ast::Visitor::visitFunctionDecl(ast::FunctionDecl &function) { visitDeclaration(function); }
void ast::Visitor::visitTypeAlias(ast::TypeAliasDecl &alias) { visitDeclaration(alias); }

void ast::Visitor::visitEnumDecl(ast::EnumDecl &enumDecl) { visitDeclaration(enumDecl); }
void ast::Visitor::visitClassDecl(ast::ClassDecl &classDecl) { visitNameSpace(classDecl); }
void ast::Visitor::visitFieldDecl(ast::FieldDecl &fieldDecl) { visitDeclaration(fieldDecl); }
void ast::Visitor::visitProtocolDecl(ast::ProtocolDecl &protocolDecl) { visitDeclaration(protocolDecl); }


void ast::Visitor::visitStatement(ast::Stmt &statement) { visitComponent(statement); }
void ast::Visitor::visitCompoundStatement(ast::CompoundStmt &statement) { visitStatement(statement); }
void ast::Visitor::visitVarDeclStatement(ast::VarDeclStmt &statement) { visitStatement(statement); }
void ast::Visitor::visitReturnStatement(ast::ReturnStmt &statement) { visitStatement(statement); }
void ast::Visitor::visitBreakStatement(ast::BreakStmt &statement) { visitStatement(statement); }
void ast::Visitor::visitContinueStatement(ast::ContinueStmt &statement) { visitStatement(statement); }

void ast::Visitor::visitIfStatement(ast::IfStmt &statement) { visitStatement(statement); }
void ast::Visitor::visitSwitchStatement(ast::SwitchStmt &statement) { visitStatement(statement); }

void ast::Visitor::visitSimpleIterationStatement(ast::SimpleIterStmt &statement) { visitStatement(statement); }
void ast::Visitor::visitPreWhileStatement(ast::PreWhileStmt &statement) { visitSimpleIterationStatement(statement); }
void ast::Visitor::visitPreUntilStatement(ast::PreUntilStmt &statement) { visitSimpleIterationStatement(statement); }
void ast::Visitor::visitPostWhileStatement(ast::PostWhileStmt &statement) { visitSimpleIterationStatement(statement); }
void ast::Visitor::visitPostUntilStatement(ast::PostUntilStmt &statement) { visitSimpleIterationStatement(statement); }
void ast::Visitor::visitForStatement(ast::ForStmt &statement) { visitSimpleIterationStatement(statement); }


void ast::Visitor::visitExpression(ast::Expr &expression) { visitStatement(expression); }
void ast::Visitor::visitCastExpression(ast::ImplicitCastExpr &cast) { visitExpression(cast); }
void ast::Visitor::visitEvalExpression(ast::EvalExpr &cast) { visitExpression(cast); }

void ast::Visitor::visitBinaryExpression(ast::BinaryExpr &expression) { visitExpression(expression); }
void ast::Visitor::visitArithmeticExpression(ast::ArithmeticExpr &expression) { visitBinaryExpression(expression); }
void ast::Visitor::visitComparisonExpression(ast::ComparisonExpr &expression) { visitBinaryExpression(expression); }
void ast::Visitor::visitAssignment(ast::AssignmentExpr &expression) { visitBinaryExpression(expression); }
void ast::Visitor::visitBitwiseExpression(ast::BitwiseExpr &expression) { visitBinaryExpression(expression); }

void ast::Visitor::visitUnaryExpression(ast::UnaryExpr &expression) { visitExpression(expression); }
void ast::Visitor::visitArithmeticNegationExpression(ast::ArithmeticNegationExpr &expression) { visitUnaryExpression(expression); }
void ast::Visitor::visitLogicalNegationExpression(ast::LogicalNegationExpr &expression) { visitUnaryExpression(expression); }
void ast::Visitor::visitBitwiseNegationExpression(ast::BitwiseNegationExpr &expression) { visitUnaryExpression(expression); }

void ast::Visitor::visitVarRef(ast::VarRef &varRef) { visitExpression(varRef); }
void ast::Visitor::visitFunctionCall(ast::FunctionCall &functionCall) { visitExpression(functionCall); }
void ast::Visitor::visitFieldRef(ast::FieldRef &fieldRef) { visitExpression(fieldRef); }

void ast::Visitor::visitConstant(ast::Constant &constant) { visitExpression(constant); }
void ast::Visitor::visitCharLiteral(ast::CharLiteral &literal) { visitConstant(literal); }
void ast::Visitor::visitIntegerLiteral(ast::IntegerLiteral &literal) { visitConstant(literal); }
void ast::Visitor::visitUIntegerLiteral(ast::UIntegerLiteral &literal) { visitConstant(literal); }
void ast::Visitor::visitLongLiteral(ast::LongLiteral &literal) { visitConstant(literal); }
void ast::Visitor::visitULongLiteral(ast::ULongLiteral &literal) { visitConstant(literal); }
void ast::Visitor::visitFloatLiteral(ast::FloatLiteral &literal) { visitConstant(literal); }
void ast::Visitor::visitDoubleLiteral(ast::DoubleLiteral &literal) { visitConstant(literal); }
void ast::Visitor::visitBoolLiteral(ast::BoolLiteral &literal) { visitConstant(literal); }
void ast::Visitor::visitStringLiteral(ast::StringLiteral &literal) { visitConstant(literal); }
void ast::Visitor::visitNullPointer(ast::NullPointer &nullpointer) { visitConstant(nullpointer); }
*/
