//
//  Stmt.h
//  Human Plus
//
//  Created by Lore on 20/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#ifndef __human_plus_compiler_ast_statement
#define __human_plus_compiler_ast_statement

#include <hpc/ast/component.h>

#include <vector>

namespace hpc {
    namespace ast {
        
        class Expr;
        class Var;
        class FunctionDecl;
        class Type;
        
        /*!
         \brief Values which indicates possible statement roles given to \c break statements.
         */
        typedef enum : unsigned {
            BreakUnable  = 0,
            BreakDefault = ~0u,
            BreakDoWhile = 1,
            BreakDoUntil = 2,
            BreakDo = 1|2,
            BreakWhile = 4,
            BreakUntil = 8,
            BreakFor = 16,
            BreakSwitch = 32,
            BreakSelect = 64
        } break_target;
        
        /*!
         \brief Values which indicates possible statement roles given to \c continue statements.
         */
        typedef enum : unsigned {
            ContinueUnable  = 0,
            ContinueDefault = ~0u,
            ContinueDoWhile = 1,
            ContinueDoUntil = 2,
            ContinueDo = 1|2,
            ContinueWhile = 4,
            ContinueUntil = 8,
            ContinueFor = 16,
        } continue_target;
        
        /*!
         \brief Base class for all the statements represented in the AST.
         */
        class Stmt : public Component {
            
        public:
            /*!
             \brief Returns a boolean indicating whether <b>all</b> the possible paths provided by the statement end up with a return statement.
             \note This method will count other statements which will make the control leave the function such as exception throwers.
             */
            virtual bool returns() const { return false; }
            /*!
             \brief Returns the number of return statements contained in this statement.
             \note Unlike \c returns(), this method won't count exceptional returning statements such as exception throwers.
             */
            virtual int containedReturns() const { return 0; }
            
            /*!
             \brief The role \c break statements will use to refer this kind of statement, or \c BreakUnable if this statement does not support break.
             */
            virtual break_target getBreakRole() const { return BreakUnable; }
            /*!
             \brief The role \c continue statements will use to refer this kind of statement, or \c ContinueUnable if this statement does not support continue.
             */
            virtual continue_target getContinueRole() const { return ContinueUnable; }
            

            llvm_rtti_impl_superclass(Stmt);
        };
        
        /*!
         \brief An object describing a compound statement, a statement which can contain other statements that will be executed in sequence.
         \note In Human Plus, compound statements are specified in the source code with a C-like pair of braces <tt>{ }</tt>
         */
        class CompoundStmt : public Stmt {
            /*!
             \brief A \c std::vector with all the statements contained by the compound statement.
             */
            std::vector<Stmt *> substatements;
            
        public:
            /*!
             \brief Initializes the compound statement as an empty scope.
             */
            CompoundStmt() {  }
            virtual ~CompoundStmt() {  }
            
            /*!
             \brief Returns a \c std::vector containing the sequence of the contained statements.
             */
            inline const std::vector<Stmt *> &statements() const { return substatements; }
            
            /*!
             \brief Inserts the given statement into the compound statement.
             */
            void pushSubStatement(Stmt *statement);
            
            virtual bool returns() const;
            virtual int containedReturns() const;

            
            llvm_rtti_impl(CompoundStmt);
        };
        
        /*!
         \brief An object describing a local \c let declaration statement.
         */
        class VarDeclStmt : public Stmt {
            /*!
             \brief The function this declaration statement is contained in.
             */
            FunctionDecl *container;
            /*!
             \brief A \c std::vector containing the variables declared by this statement.
             */
            std::vector<Var *> variables;
            
        public:
            /*!
             \brief Initializes an empty local declaration statement with it container function.
             */
            VarDeclStmt(FunctionDecl *container);
            virtual ~VarDeclStmt() {  }
            
            inline FunctionDecl *getContainerFunction() const { return container; }
            
            inline const std::vector<Var *> &getDeclaredVariables() const { return variables; }
            
            /*!
             \brief Adds the given variable as declared by the declaration statement.
             */
            void addVariable(Var *var);
            

            llvm_rtti_impl(VarDeclStmt);
        };
        
        /*!
         \brief An object describing a basic \c return statement.
         */
        class ReturnStmt : public Stmt {
            /*!
             \brief The function this return statement is contained in.
             */
            FunctionDecl *container;
            /*!
             \brief The value this statement will return, or \c nullptr for void returns.
             */
            Expr *returnVal;
            
        public:
            /*!
             \brief Initializes the return statement with the container function and a value this statement will return.
             */
            ReturnStmt(FunctionDecl *container, Expr *returnVal = nullptr);
            virtual ~ReturnStmt() {  }
            
            inline FunctionDecl *getReceiver() const { return container; }
            
            inline Expr *getReturnValue() const { return returnVal; }
            
            void castReturnValueToType(ast::Type *destination);
            
            virtual bool returns() const { return true; }
            virtual int containedReturns() const { return 1; }
            

            llvm_rtti_impl(ReturnStmt);
        };
        
        /*!
         \brief An object describing a \c break statement.
         */
        class BreakStmt : public Stmt {
            /*!
             \brief The statement this statement will break.
             */
            Stmt *container = nullptr;
            
        public:
            /*!
             \brief Initializes a break statement.
             */
            BreakStmt();
            virtual ~BreakStmt() {  }
            
            inline Stmt *getBreakCatcher() const { return container; }
            
            inline void setBreakCatcher(Stmt *newCatcher) { container = newCatcher; }

            
            llvm_rtti_impl(BreakStmt);
        };
        
        /*!
         \brief An object describing a \c continue statement.
         */
        class ContinueStmt : public Stmt {
            /*!
             \brief The statement this statement will continue.
             */
            Stmt *container = nullptr;
            
        public:
            /*!
             \brief Initializes a continue statement.
             */
            ContinueStmt();
            virtual ~ContinueStmt() {  }
            
            inline Stmt *getContinueCatcher() const { return container; }
            
            inline void setContinueCatcher(Stmt *newCatcher) { container = newCatcher; }
            

            llvm_rtti_impl(ContinueStmt);
        };
        
    }
}

#endif
