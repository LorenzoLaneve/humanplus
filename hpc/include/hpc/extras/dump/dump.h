// => hpc/extras/dump/dump.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_extras_ast_dump
#define __human_plus_compiler_extras_ast_dump

#include <hpc/utils/printers.h>
#include <hpc/utils/strings.h>
#include <hpc/ast/ast.h>
#include <hpc/ast/visitor.h>

#include <llvm/Support/raw_ostream.h>

#include <vector>

namespace hpc {
    
    namespace extras {
        
        /*!
         \brief Visitor class which prints an AST to a given text stream. The following format is used:
         \code
         CompilationUnit
         |-Elem1
         | `-Elem1.1
         `-Elem2
         \endcode
         */
        class NewASTPrinter : public ast::RecursiveVisitor<NewASTPrinter> {
            
            llvm::raw_ostream &os;
            
            std::vector<unsigned long> branches;
            
        public:
            NewASTPrinter(llvm::raw_ostream &os = llvm::outs()) : os(os) {  }
            
            /*!
             \brief Prints the whole AST using the format of the visitor.
             */
            void print(ast::AbstractSyntaxTree &theAST);
            
            /*!
             \brief Adds a branch with the given number of children. The next \c children components will be printed as children of the last printed AST component.
             */
            void openChildBranch(unsigned long children);
            /*!
             \brief Removes the last branch.
             */
            void closeLastChildBranch();
            /*!
             \brief Prints the branch for the next component.
             */
            void printBranch();
            
            /*!
             \brief Prints a new branch with the information of the given AST object.
             */
            inline void printObject(std::string className, ast::Component *component) {
                printBranch();
                if (os.has_colors()) os.changeColor(component->isValid() ? llvm::raw_ostream::SAVEDCOLOR : llvm::raw_ostream::RED, true);
                os << className;
                if (os.has_colors()) os.changeColor(llvm::raw_ostream::YELLOW);
                os << " " << component << " ";
                if (os.has_colors()) os.resetColor();
            }
            
            
            void visitType(ast::Type *type);
            void visitTypeRef(ast::TypeRef *typeRef);
            
            void visitNameSpaceDecl(ast::NameSpaceDecl *nameSpace);
            void visitFunctionDecl(ast::FunctionDecl *function);
            void visitEnumDecl(ast::EnumDecl *enumDecl);
            void visitClassDecl(ast::ClassDecl *classDecl);
            void visitProtocolDecl(ast::ProtocolDecl *protocolDecl);
            void visitTypeAliasDecl(ast::TypeAliasDecl *alias);
            
            void visitCompoundStmt(ast::CompoundStmt *statement);
            void visitVarDeclStmt(ast::VarDeclStmt *statement);
            void visitReturnStmt(ast::ReturnStmt *statement);
            void visitBreakStmt(ast::BreakStmt *statement);
            void visitContinueStmt(ast::ContinueStmt *statement);
            void visitIfStmt(ast::IfStmt *statement);
            void visitSimpleIterStmt(ast::SimpleIterStmt *statement);
            void visitPreWhileStmt(ast::PreWhileStmt *statement);
            void visitPreUntilStmt(ast::PreUntilStmt *statement);
            void visitPostWhileStmt(ast::PostWhileStmt *statement);
            void visitPostUntilStmt(ast::PostUntilStmt *statement);
            void visitForStmt(ast::ForStmt *statement);
            
            void visitImplicitCastExpr(ast::ImplicitCastExpr *expression);
            void visitEvalExpr(ast::EvalExpr *expression);
            
            void visitBinaryExpr(ast::BinaryExpr *expression);
            void visitUnaryExpr(ast::UnaryExpr *expression);
            void visitCharLiteral(ast::CharLiteral *literal);
            void visitStringLiteral(ast::StringLiteral *literal);
            void visitIntegerLiteral(ast::IntegerLiteral *literal);
            void visitUIntegerLiteral(ast::UIntegerLiteral *literal);
            void visitLongLiteral(ast::LongLiteral *literal);
            void visitULongLiteral(ast::ULongLiteral *literal);
            void visitFloatLiteral(ast::FloatLiteral *literal);
            void visitDoubleLiteral(ast::DoubleLiteral *literal);
            void visitBoolLiteral(ast::BoolLiteral *literal);
            void visitNullPointer(ast::NullPointer *nullpointer);
            void visitVarRef(ast::VarRef *varRef);
            void visitFunctionCall(ast::FunctionCall *functionCall);
            void visitFieldRef(ast::FieldRef *fieldRef);
            void visitGlobalVar(ast::GlobalVar *var);
            void visitLocalVar(ast::LocalVar *var);
            void visitParamVar(ast::ParamVar *var);
            void visitFieldDecl(ast::FieldDecl *fieldDecl);
            
        };

        /*!
         \brief Prints the whole AST in the format below.
         \code
         AbstractSyntaxTree
         |-Elem1
         | `-Elem1.1
         `-Elem2
         \endcode
         */
        void dumpAST(ast::AbstractSyntaxTree *ast);
        
    }
    
}

#endif
