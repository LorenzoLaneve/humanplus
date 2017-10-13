//
//  FunctionDecl.h
//  Human Plus
//
//  Created by Lore on 11/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#ifndef __human_plus_compiler_ast_function
#define __human_plus_compiler_ast_function

#include <hpc/ast/decls/variable.h>
#include <hpc/ast/unit.h>
#include <hpc/ast/stmt/statement.h>

#define __human_plus_main_function_identifier "main"

namespace hpc {
    namespace ast {
        
        /*!
         \brief An object describing a simple function.
         */
        class FunctionDecl : public GlobalDecl {
            
        public:
            /*!
             \brief A structure containing additional attributes for a function used by the compiler.
             */
            struct FunctionAttributes {
                bool nostalgic = false;     ///< Nostalgic function will be linked as C functions, without being mangled.
                //bool onewayf = false;     ///< Oneway functions will be called asynchronously
                
                FunctionAttributes() {  }
            };
            
            typedef std::vector<ast::ParamVar *> param_vector;
            
            typedef param_vector::iterator param_iterator;
            
        private:

            /*!
             \brief The function return type.
             */
            Type *returnType;
            /*!
             \brief The function arguments as \c ast::ParamVar 's.
             */
            param_vector arguments;
            /*!
             \brief A \c std::vector containing all the local variables declared in this function.
             */
            std::vector<Var *> localdecls;
            /*!
             \brief The function statements block.
             */
            CompoundStmt *statements;
            
            /*!
             \brief The number of \c return statements contained in the function block. This member is a cache for the \c containedReturns() method.
             */
            int retcount = -1;
            
            /*!
             \brief A \c FunctionAttributes structure describing all the additional attributes that can be given to a function.
             \see \c FunctionAttributes structure for possible attributes the function can take.
             */
            FunctionAttributes fattrs;
            
        public:
            /*!
             \brief Initializes the function.
             \param name The function identifier (without the container identifier)
             \param arguments A \c std::vector of \c ast::ParamVar objects each describing a parameter the function will take
             \param rtype The function return type
             \param fattrs An optional \c FunctionAttributes structure containing the additional attributes of the function
             \param statements An \c ast::CompoundStmt object describing a block of statements for the function, or \c nullptr for external function declarations
             \see \c FunctionAttributes structure for possible attributes the function can take.
             */
            FunctionDecl(std::string name, std::vector<ParamVar *> arguments, Type *returnType, FunctionAttributes fattrs = {}, CompoundStmt *statements = nullptr)
                : GlobalDecl(name), returnType(returnType), arguments(arguments), statements(statements), fattrs(fattrs) {  }
            
            virtual ~FunctionDecl() {  }

            /*!
             \brief Returns the function return type.
             */
            inline Type *getReturnType() const { return returnType; }
            /*!
             \brief Returns an \c ast::CompoundStmt object describing the function statements block, or \c nullptr if the function is external.
             */
            inline CompoundStmt *getStatementsBlock() const { return statements; }
            
            /*!
             \brief Sets the function statements block to the given block.
             */
            void setStatementsBlock(CompoundStmt *stg);
            
            /*!
             \brief Add the local declaration to this function.
             */
            void addLocalDeclaration(Var *variable);
            /*!
             \brief Returns all the variables declared within the local scope of this function.
             */
            inline const std::vector<ast::Var *> &getLocalVars() const { return localdecls; }

            /*!
             \brief Returns a \c std::vector containing the functions parameters as \c ast::ParamVar objects.
             */
            inline const std::vector<ParamVar *> &getArgs() const { return arguments; }
            
            /*!
             \brief Returns the number of \c return statements contained in the function statements block.
             */
            int containedReturns();

            /*!
             \brief A boolean indicating whether this function has the qualified name to be the program's main function.
             \note This method is useful when we need to determine whether we should mangle a function name, or whether we should just mark it as \c main.
             */
            bool isMainFunction() const;
            
            /*!
             \brief A boolen indicating whether this function is marked as \c nostalgic.
             \note In Human Plus, a \c nostalgic function is a function which name will not be mangled, so it will be able to be easily linked with C functions. This Human Plus feature is for retrocompatibility and interoperability with C.
             */
            bool isNostalgic() const;
            
            
            llvm_rtti_impl(FunctionDecl);
        };
        
    }
}

#endif
