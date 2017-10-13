// => hpc/ast/exprs/reference.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_reference
#define __human_plus_compiler_ast_reference

#include <hpc/ast/exprs/expression.h>
#include <hpc/ast/symbols.h>
#include <hpc/ast/decls/function.h>

#include <llvm/IR/Module.h>

namespace hpc {
    namespace ast {
        
        /*!
         \brief An object describing a reference to a variable entity.
         */
        class VarRef : public Expr {
            /*!
             \brief The \c ast::Symbol the validator will use to resolve this variable reference.
             */
            Symbol pathToVar;
            /*!
             \brief The variable this object is refering to.
             \note This will be \c nullptr until \c validate() is called.
             */
            Var *variable = nullptr;
            
        public:
            /*!
             \brief Initializes the reference with an \c ast::Symbol to be resolved.
             */
            VarRef(Symbol pathToVar) : pathToVar(pathToVar) {  }
            virtual ~VarRef() {  }
            
            inline const Symbol &getSymbol() const { return pathToVar; }
            
            inline Var *getVar() const { return variable; }
            
            inline void setVar(Var *newVar) { variable = newVar; }

            virtual Type *evalType();
            
            virtual bool supportsLeftHandAssignment() const { return true; }
            
            
            llvm_rtti_impl(VarRef);
        };
        
        /*!
         \brief An object describing a call to a function entity.
         */
        class FunctionCall : public Expr {
            /*!
             \brief The \c ast::Symbol the validator will use to resolve this function reference.
             */
            Symbol pathToFunc;
            /*!
             \brief The function this object is refering to.
             \note This will be \c nullptr until \c validate() is called.
             */
            FunctionDecl *prototype = nullptr;
            /*!
             \brief A \c std::vector containing all the values that will be passed as parameter to the function.
             */
            std::vector<Expr *> actualParams;
            
        public:
            /*!
             \brief Initializes the reference with an \c ast::Symbol to be resolved.
             */
            FunctionCall(Symbol pathToFunc) : pathToFunc(pathToFunc) {  };
            virtual ~FunctionCall() {  }
            
            inline const ast::Symbol &getSymbol() const { return pathToFunc; }
            
            inline ast::FunctionDecl *getFunctionDecl() const { return prototype; }
            
            inline void setFunctionDecl(ast::FunctionDecl *newPrototype) { prototype = newPrototype; }
            
            inline const std::vector<Expr *> &getActualParams() const { return actualParams; }
            
            void castActualParamToType(int i, ast::Type *destination);
            
            virtual Type *evalType();
            
            /*!
             \brief Adds a value to the list of parameters to be passed to the function.
             */
            virtual void addCallParameter(Expr *parameter);
            //virtual bool supportsLeftHandAssignment() { //C++ like reference returning. }
            
            
            llvm_rtti_impl(FunctionCall);
        };
    }
}

#endif
