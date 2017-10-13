// => hpc/analyzers/validator/validator.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_validator
#define __human_plus_compiler_validator

#include <hpc/ast/ast.h>
#include <hpc/ast/visitor.h>
#include <hpc/analyzers/validator/resolver.h>
#include <hpc/diagnostics/diagnostics.h>

#include <string>
#include <vector>
#include <map>

namespace hpc {
    namespace validator {
        
        /*!
         \brief Object which validates a Syntax Tree and transforms it into an AST.
         */
        class ValidatorInstance {
            /*!
             \brief The diagnostics engine this compilation has to report diagnostics to.
             */
            diag::DiagEngine &diags;
        
        public:
            virtual ~ValidatorInstance() {  }
            
            ValidatorInstance(diag::DiagEngine &diags);
            
            inline diag::DiagEngine &getDiags() const {
                return diags;
            }
            
            /*!
             \brief Starts a validation session on the given AST.
             */
            bool validate(ast::AbstractSyntaxTree *ast);

        };
        
        class ValidatorImpl : public ast::RecursiveVisitor<ValidatorImpl> {
            
            /*!
             \brief The validator instance for this implementation.
             */
            ValidatorInstance &validator;
            /*!
             \brief The diagnostics engine that will take any validation diagnostics.
             */
            diag::DiagEngine &diags;
            /*
             \brief The AST being validated.
             */
            ast::AbstractSyntaxTree *ast;
            /*!
             \brief The SymbolPrevResolver object that will be used to resolve references.
             */
            SymbolResolver *resolver;
            
        public:
            ValidatorImpl(ValidatorInstance &validator, ast::AbstractSyntaxTree *ast);
            
            inline SymbolResolver &getResolver() const {
                assert(resolver && "No resolver bound.");
                return *resolver;
            }
            
            inline bool validate(ast::Component *component) {
                take(component);
                return component->isValid();
            }
            
            inline bool validate(ast::Component &component) {
                return validate(&component);
            }
            
            /*!
             \brief Returns whether the AST has been declared valid by the validator implementation.
             \note Before starting the process this method will return \c true.
             */
            inline bool validationPassed() const {
                return ast->getRootNameSpace()->isValid();
            }
            
            
            void visitAST(ast::AbstractSyntaxTree &ast);
            
            void visitUnit(ast::CompilationUnit &unit);
            
            void visitTypeRef(ast::TypeRef *typeRef);
            
            void visitPointerType(ast::PointerType *type);
            void visitClassType(ast::ClassType *type);
            
            void visitNameSpaceDecl(ast::NameSpaceDecl *nameSpace);
            
            void visitVar(ast::Var *var);
            void visitGlobalVar(ast::GlobalVar *var);
            void visitLocalVar(ast::LocalVar *var);
            void visitParamVar(ast::ParamVar *var);
            
            void visitFunctionDecl(ast::FunctionDecl *function);
            void visitTypeAliasDecl(ast::TypeAliasDecl *alias);
            
            void visitClassDecl(ast::ClassDecl *classDecl);
            void visitProtocolDecl(ast::ProtocolDecl *protocolDecl);
            
            
            void visitCompoundStmt(ast::CompoundStmt *statement);
            void visitVarDeclStmt(ast::VarDeclStmt *statement);
            void visitReturnStmt(ast::ReturnStmt *statement);
            void visitBreakStmt(ast::BreakStmt *statement);
            void visitContinueStmt(ast::ContinueStmt *statement);
            
            void visitIfStmt(ast::IfStmt *statement);
            //void visitSwitchStmt(ast::SwitchStmt *statement) {  }
            
            void visitSimpleIterStmt(ast::SimpleIterStmt *statement);
            void visitForStmt(ast::ForStmt *statement);
            
            void visitImplicitCastExpr(ast::ImplicitCastExpr *cast);
            void visitEvalExpr(ast::EvalExpr *cast);
            
            void visitBinaryExpr(ast::BinaryExpr *expression);
            void visitAssignmentExpr(ast::AssignmentExpr *expression);
            void visitBitwiseExpr(ast::BitwiseExpr *expression);
            
            void visitArithmeticNegationExpr(ast::ArithmeticNegationExpr *expression);
            void visitLogicalNegationExpr(ast::LogicalNegationExpr *expression);
            void visitBitwiseNegationExpr(ast::BitwiseNegationExpr *expression);
            
            
            void visitVarRef(ast::VarRef *varRef);
            void visitFunctionCall(ast::FunctionCall *functionCall);
            void visitFieldRef(ast::FieldRef *fieldRef);
            
            void visitCharLiteral(ast::CharLiteral *literal) {  }
            void visitIntegerLiteral(ast::IntegerLiteral *literal) {  }
            void visitUIntegerLiteral(ast::UIntegerLiteral *literal) {  }
            void visitLongLiteral(ast::LongLiteral *literal) {  }
            void visitULongLiteral(ast::ULongLiteral *literal) {  }
            void visitFloatLiteral(ast::FloatLiteral *literal) {  }
            void visitDoubleLiteral(ast::DoubleLiteral *literal) {  }
            void visitBoolLiteral(ast::BoolLiteral *literal) {  }
            void visitStringLiteral(ast::StringLiteral *literal) {  }
            void visitNullPointer(ast::NullPointer *nullpointer) {  }
            
        };
        
        /*!
         \brief Object containing all the information needed for the validation session.
         */
        struct ValidationSession {
            /*!
             \brief The diagnostics engine this validation session has to report diagnostics to.
             */
            diag::DiagEngine &diags;
            /*!
             \brief The AST object this validation session is operating on.
             */
            ast::CompilationUnit *ast;
            /*!
             \brief The symbol resolver used to resolve entity references in the code.
             */
            SymbolPrevResolver *resolver;
            
        public:
            ValidationSession(diag::DiagEngine &diags, ast::CompilationUnit *ast);
        };
        
    }
}

#endif

