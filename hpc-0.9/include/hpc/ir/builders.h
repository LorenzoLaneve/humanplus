//
//  builders.hpp
//  HumanPlus
//
//  Created by Lore on 11/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_ir_builder
#define __human_plus_compiler_ir_builder

#include <hpc/ir/modules.h>
#include <hpc/ast/visitor.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/target/target.h>
#include <hpc/runtime/runtime.h>

#include <llvm/IR/IRBuilder.h>

#include <string>
#include <vector>
#include <map>

namespace hpc {
    
    typedef llvm::IRBuilder<> llvmir_builder;
    
    namespace codegen {
        
        class ModuleBuilder;
        class InstructionBuilder;
        
        /*!
         \brief An object that keeps all the links between the AST components and LLVM IR values.
         */
        class SymbolTable {
            
            ModuleBuilder &moduleBuilder;
            
            /*!
             \brief It keeps all the links between any already translated AST component with their IR counterpart.
             \note Use \c setValForComponent() and \c getValForComponent() to use this table.
             */
            std::map<ast::Component *, llvm::Value *> componentTable;
            /*!
             \brief It keeps all the links between any already translated AST type with their IR counterpart.
             \note Use \c getIRType() and \c setIRType() to use this table.
             */
            std::map<ast::Type *, llvm::Type *> typeTable;
            
            /*!
             \brief It keeps all the links between the strings value and the string constants declared in the module being built.
             \note Use \c getOrCreateStringConstant()
             */
            std::map<runtime::string_ty, llvm::GlobalVariable *> stringTable;
            /*!
             \brief It keeps all the links between a \c break receiver, and the block any break statement should jump to.
             \note Use \c setBreakBlock() and \c makeBreak()
             */
            std::map<ast::Stmt *, llvm::BasicBlock *> breakTable;
            /*!
             \brief It keeps all the links between a \c continue receiver, and the block any break statement should jump to.
             \note Use \c setContinueBlock() and \c makeContinue()
             */
            std::map<ast::Stmt *, llvm::BasicBlock *> continueTable;
            
            
        public:
            SymbolTable(ModuleBuilder &moduleBuilder);
            
            void setValForComponent(ast::Component *component, llvm::Value *value);
            void setValForComponent(ast::Component &component, llvm::Value *value);
            
            llvm::Value *getValForComponent(ast::Component *component);
            llvm::Value *getValForComponent(ast::Component &component);
            
            void setIRType(ast::Type *type, llvm::Type *irtype);
            void setIRType(ast::Type &type, llvm::Type *irtype);
            
            llvm::Type *getIRType(ast::Type *type);
            llvm::Type *getIRType(ast::Type &type);
            
            llvm::Value *getOrCreateReference(ast::Component *component);
            llvm::Value *getOrCreateReference(ast::Component &component);
            
            void setBreakBlock(ast::Stmt &stmt, llvm::BasicBlock *block);
            void setContinueBlock(ast::Stmt &stmt, llvm::BasicBlock *block);
            
            llvm::GlobalVariable *getOrCreateStringConstant(runtime::string_ty str);
            
            llvm::BranchInst *makeBreak(ast::Stmt *stmt);
            llvm::BranchInst *makeContinue(ast::Stmt *stmt);
            
        };
        
        class InstructionBuilder : public llvm::IRBuilder<> {
            
        public:
            InstructionBuilder(llvm::LLVMContext &context) : llvm::IRBuilder<>(context) {  }
            
            /*!
             \brief Builds a \c br instruction <b>only if</b> the insert block does not have a terminator.
             \param block The IR block the new \c br instruction will jump to.
             */
            llvm::BranchInst *CreateTerminatorIfNeeded(llvm::BasicBlock *nextBlock) {
                if (!GetInsertBlock()->getTerminator()) {
                    return CreateBr(nextBlock);
                }
                return nullptr;
            }
            
        };
        
        class ModuleBuilder : public ast::RecursiveVisitor<ModuleBuilder> {
            
            /*!
             \brief The wrapper for the module to build.
             */
            modules::ModuleWrapper &moduleWrapper;
            /*!
             \brief The TargetInfo object containing info about the target we're generating code for.
             */
            target::TargetInfo &targetInfo;
            
            SymbolTable table;
            
            /*!
             \brief The block containing the \c ret instruction. This is \c nullptr unless the builder is building a function with more than one return statement.
             */
            llvm::BasicBlock *returnBlock = nullptr;
            /*!
             \brief The LLVM IR value that will be returned. This is \c nullptr unless the builder is building a function with more than one return statement.
             */
            llvm::Value *returnRegister = nullptr;
            
            /*!
             \brief The LLVM IR builder used to create instructions. This is \c nullptr unless the builder is building a function.
             */
            InstructionBuilder *builder = nullptr;
            
        public:
            ModuleBuilder(modules::ModuleWrapper &moduleWrapper, target::TargetInfo &targetInfo);
            virtual ~ModuleBuilder() {  }
            
            inline llvm::Module &getModule() const {
                return moduleWrapper.getModule();
            }
            
            inline InstructionBuilder &getInstBuilder() const {
                assert(builder && "No builder ready.");
                return *builder;
            }
            
            inline void buildUnit(ast::CompilationUnit *unit) {
                assert(unit && "Passing nullptr as unit.");
                visitUnit(*unit);
            }
            
        private:
            llvm::Value *build(ast::Component *component) {
                if (!component) return nullptr;
                
                take(component);
                return table.getValForComponent(component);
            }
            
            llvm::StoreInst *assign(ast::Var &var, llvm::Value *val) {
                if (var.getType()->isBooleanType()) {
                    val = builder->CreateZExt(val, table.getIRType(var.getType()));
                }
                return builder->CreateStore(val, table.getOrCreateReference(var));
            }
            
            inline llvm::StoreInst *assign(ast::Var *var, llvm::Value *val) {
                return assign(*var, val);
            }
            
            inline llvm::Type *getIRType(ast::Type *type) {
                return table.getIRType(type->getCanonicalType());
            }
            
            
        public:
            
            inline target::TargetInfo &getTargetInfo() const {
                return targetInfo;
            }
            
            void visitUnit(ast::CompilationUnit &unit);

            void visitBuiltinType(ast::BuiltinType *type);
            void visitPointerType(ast::PointerType *type);
            void visitClassType(ast::ClassType *type);
            void visitQualifiedType(ast::QualifiedType *type);
            
            void visitNameSpaceDecl(ast::NameSpaceDecl *nameSpace);
            
            void visitGlobalVar(ast::GlobalVar *var);
            
            void visitFunctionDecl(ast::FunctionDecl *function);
            
            void visitClassDecl(ast::ClassDecl *classDecl);
            
            
            void visitCompoundStmt(ast::CompoundStmt *statement);
            void visitVarDeclStmt(ast::VarDeclStmt *statement);
            void visitReturnStmt(ast::ReturnStmt *statement);
            void visitBreakStmt(ast::BreakStmt *statement);
            void visitContinueStmt(ast::ContinueStmt *statement);
            
            void visitIfStmt(ast::IfStmt *statement);
            //void visitSwitchStmt(ast::SwitchStmt *statement) {  }
            
            void visitPreWhileStmt(ast::PreWhileStmt *statement);
            void visitPreUntilStmt(ast::PreUntilStmt *statement);
            void visitPostWhileStmt(ast::PostWhileStmt *statement);
            void visitPostUntilStmt(ast::PostUntilStmt *statement);
            void visitForStmt(ast::ForStmt *statement);
            
            
            void visitImplicitCastExpr(ast::ImplicitCastExpr *cast);
            void visitEvalExpr(ast::EvalExpr *cast);
            
            void visitArithmeticExpr(ast::ArithmeticExpr *expression);
            void visitComparisonExpr(ast::ComparisonExpr *expression);
            void visitAssignmentExpr(ast::AssignmentExpr *expression);
            void visitBitwiseExpr(ast::BitwiseExpr *expression);
            
            void visitArithmeticNegationExpr(ast::ArithmeticNegationExpr *expression);
            void visitLogicalNegationExpr(ast::LogicalNegationExpr *expression);
            void visitBitwiseNegationExpr(ast::BitwiseNegationExpr *expression);
            
            
            void visitVarRef(ast::VarRef *varRef);
            void visitFunctionCall(ast::FunctionCall *functionCall);
            void visitFieldRef(ast::FieldRef *fieldRef);
            
            void visitCharLiteral(ast::CharLiteral *literal);
            void visitIntegerLiteral(ast::IntegerLiteral *literal);
            void visitUIntegerLiteral(ast::UIntegerLiteral *literal);
            void visitLongLiteral(ast::LongLiteral *literal);
            void visitULongLiteral(ast::ULongLiteral *literal);
            void visitFloatLiteral(ast::FloatLiteral *literal);
            void visitDoubleLiteral(ast::DoubleLiteral *literal);
            void visitBoolLiteral(ast::BoolLiteral *literal);
            void visitStringLiteral(ast::StringLiteral *literal);
            void visitNullPointer(ast::NullPointer *nullpointer);
            
        };

    }
}

#endif
