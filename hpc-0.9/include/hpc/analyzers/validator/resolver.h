//
//  resolver.h
//  HumanPlus
//
//  Created by Lore on 15/12/2016.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_validator_resolver
#define __human_plus_compiler_validator_resolver

#include <hpc/ast/decls/namespace.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/diagnostics/diagnostics.h>

#include <vector>
#include <map>

namespace hpc {
    namespace validator {
        
        namespace lookup {
            /*!
             \brief Parameters used to look for an entity.
             */
            typedef enum {
                /*!
                 \brief Tells the LocalStack object to only look for the corresponding variable in the local scope.
                 */
                Local = 1,
                /*!
                 \brief Tells the LocalStack object to only look for the corresponding variable in the namespace the validator is currently controlling.
                 */
                Global = 2,
                /*!
                 \brief Tells the LocalStack object to look for the corresponding variable in both local and global scopes.
                 \note It is equivalent to \c LookupLocal|LookupGlobal
                 */
                Both = 1|2,
            } LookupParams;
        }
        
        class ValidatorInstance;
        class LocalStack;
        
        typedef std::vector<ast::FunctionDecl *> CandidateFunctions;
        
        /*!
         \brief Object which resolves high-level symbols. It provides methods that return an AST object for a given symbol.
         */
        class SymbolResolver {
            /*!
             \brief The validator instance associated to this resolver.
             */
            ValidatorInstance &validator;
            /*!
             \brief The AST this resolver should lookup in.
             */
            ast::AbstractSyntaxTree &ast;
            /*!
             \brief The symbols will be resolved assuming they're in this namespace.
             */
            ast::NameSpaceDecl *currentNameSpace = nullptr;
            /*!
             \brief The Entity stack used to track function-local entities. This is \c nullptr when the validation session is not validating a function.
             */
            std::vector<LocalStack> localStacks;
            
        public:
            SymbolResolver(ValidatorInstance &validator, ast::AbstractSyntaxTree &ast);
            
            /*!
             \brief Returns the last local stack opened for the current function validation.
             \note If no function is being validated. This method will \c assert.
             */
            inline LocalStack &getInnermostStack() {
                return localStacks.back();
            }
            
            /*!
             \brief Switches the current namespace to the given namespace.
             */
            void switchTo(ast::NameSpaceDecl *nameSpace);
            /*!
             \brief Switches the current namespace to the container namespace.
             \note If the container is \c nullptr for any reason, the held AST global scope is taken.
             */
            void switchToContainer();
            
            inline ast::NameSpaceDecl &getInsertNameSpace() const {
                assert(currentNameSpace && "No insert namespace.");
                return *currentNameSpace;
            }
            
            /*!
             \brief Prepares the resolver to validate the given function.
             */
            void openLocalStackForFunction(ast::FunctionDecl *F);
            /*!
             \brief Clears the last opened local stack and closes the currently analyzing function.
             */
            void closeLastLocalStack();
            
            /*!
             \brief Adds a local variable to the stack, in the innermost scope.
             \return \c true if the variable has been added successfully to the stack, \c false if this reported an error to the validator's diag engine.
             */
            bool declareVariable(ast::Var &variable);
            
            /*!
             \brief Looks in the local stacks for a variable matching \c sympath .
             \return An \c ast::Var pointing to the found variable, or \c nullptr if no matching variable is found.
             */
            ast::Var *getMatchingLocalVariable(ast::Symbol sympath);
            /*!
             \brief Looks in the global namespaces for a variable matching \c sympath .
             \return An \c ast::GlobalVar pointing to the found variable, or \c nullptr if no matching variable is found.
             */
            ast::GlobalVar *getMatchingGlobalVariable(ast::Symbol sympath, bool noReport = false);
            /*!
             \brief Looks for a matching variable, in the local scopes and then in the global namespaces.
             \return An \c ast::Var pointing to the found variable, or \c nullptr if no matching variable is found.
             */
            ast::Var *getMatchingVariable(ast::Symbol sympath, int params = lookup::Both, bool noReport = false);
            
            inline ast::GlobalVar *getMatchingGlobalVariableNoReport(ast::Symbol sympath) {
                return getMatchingGlobalVariable(sympath, true);
            }
            
            inline ast::Var *getMatchingVariableNoReport(ast::Symbol sympath, int params = lookup::Both) {
                return getMatchingVariable(sympath, params, true);
            }
            
            /*!
             \brief Looks for a namespace with the first name of the specified \c Symbol, in the current scope. If the current scope does not contain a namespace with the given name, the container will be taken and the search will restart, until the main global scope is found.
             \return An \c ast::NameSpaceDecl object describing the matching namespace, or \c nullptr if the namespace does not exist.
             \note This function <u>will not</u> report any error before returning a \c nullptr.
             */
            ast::NameSpaceDecl *getInnermostMatchingNameSpace(ast::Symbol sympath);

            /*!
             \brief Looks for all the functions matching the given \c Symbol . All the results will be put in \c candidates .
             \return \c false if the lookup generated a compilation error, \c true otherwise.
             */
            bool getMatchingCandidateFunctions(CandidateFunctions &candidates, ast::Symbol sympath, bool noReport = false);
            
            inline bool getMatchingCandidateFunctionsNoReport(CandidateFunctions &candidates, ast::Symbol sympath) {
                return getMatchingCandidateFunctions(candidates, sympath, true);
            }
            
            /*!
             \brief Looks for a type in the namespace matching the given \c ast::Symbol.
             */
            ast::Type *getMatchingType(ast::Symbol sympath, bool noReport = false);
            
            inline ast::Type *getMatchingTypeNoReport(ast::Symbol sympath) {
                return getMatchingType(sympath, true);
            }
            
        };
        
        
        /*!
         \brief Object used by the validator. Holds all the stacks with the declared entities.
         */
        class LocalStack {
        public:
            
            /*!
             \brief Defines a type describing a single local scope. A local scope is a scope where local variables can be declared. they die at the end of the same scope (E.g.: a compound statement)
             */
            typedef std::map<std::string, ast::Var *> Scope;
            /*!
             \brief Defines a type describing a local stack. A local stack is the stack of local scopes used by the \c LocalStack to keep track of the local variables in a validation session.
             */
            typedef std::vector<Scope> StackVector;
            
            /*!
             \brief Holds a stack of scopes that hold the local variables when validating a specific function.
             \note This vector <b>must</b> be cleared with \c LocalStack::clear() when the validation for a function ends.
             */
            StackVector localStack;
            
            /*!
             \brief The resolver that opened this stack.
             */
            SymbolResolver &resolver;
            /*!
             \brief The function this stack is tracking.
             */
            ast::FunctionDecl *F;
            
            /*!
             \brief Holds the statements that may potentially be broken by the \c break statement.
             */
            std::vector<ast::Stmt *> breakCatchers;
            /*!
             \brief Holds the statements that may potentially be repeated by the \c continue statement.
             */
            std::vector<ast::Stmt *> continueCatchers;
            /*!
             \brief Holds the statements that may potentially be switched by a \c case statement.
             */
            std::vector<ast::Stmt *> caseCatchers;
            
        public:
            /*!
             \brief Creates an LocalStack object ready to use for a validation session.
             \param resolver The resolver that opened this stack.
             \param F The function declaration this stack is tracking.
             */
            LocalStack(SymbolResolver &resolver, ast::FunctionDecl *F);
            
            /*!
             \brief Returns an array of the scopes contained in the LocalStack at the moment of the call to this method.
             */
            inline const StackVector &getLocalScopes() const {
                return localStack;
            }
            
            /*!
             \brief Returns the innermost scope opened in the stack.
             */
            inline Scope &getInnermostScope() {
                return localStack.back();
            }
            
            /*!
             \brief Adds a local scope to the stack.
             \note This method should be used when the control of the validator enters in statements that may isolate declarations (E.g: compound statements), where variable will "live" and "die". This behavior is achieved by creating stacks when the validation control enters a specific scope, and then remove that stack when the validation control leaves that scope.
             */
            void addScope() {
                localStack.push_back(Scope());
            }
            /*!
             \brief Removes the last scope added to the stack.
             \note This method should be used when the control of the validator enters in statements that may isolate declarations (E.g: compound statements), where variable will "live" and "die". This behavior is achieved by creating stacks when the validation control enters a specific scope, and then remove that stack when the validation control leaves that scope.
             */
            void removeScope() {
                localStack.pop_back();
            }
            
            /*!
             \brief Adds a local or param variable to the innermost scope. If there is no scope in the stack of the object, the function will \c assert.
             \param variable An \c ast::Var object that has just been declared.
             \warning You <u>should</u> add <u>only local variables</u>. This includes \c ast::LocalVar and \c ast::ParamVar objects. Adding other object types will make the validator format global variables as local variables.
             */
            void addVariable(ast::Var *variable);
            
            /*!
             \brief Clears the local scopes stack held by the \c LocalStack object.
             \note You <b>must</b> call this method when the validator finished validating a function, so the stack will be cleared for the next function. <b>Do not</b> leave any variable of a function at the end of its validation stack.
             */
            void clear();
            
            /*!
             \brief Pushes the statement in the \c break responders stack. If an enclosing \c break statement is encountered, it will make the control exit the last statement in the break stack held by the bound entity stack.
             */
            void pushBreakCatcher(ast::Stmt *responder);
            /*!
             \brief Pushes the statement in the \c break responders stack. If an enclosing \c continue statement is encountered, it will make the control repeat the last statement in the continue stack held by the bound entity stack.
             */
            void pushContinueCatcher(ast::Stmt *responder);
            /*!
             \brief Pushes the statement in the \c case responders stack. If an enclosing \c case statement is encountered, it will refer the last statement in the case stack held by the bound entity stack.
             */
            void pushCaseCatcher(ast::Stmt *responder);
            
            /*!
             \brief Removes the last statement held by the break stack. This method should always follow a \c pushBreakCatcher() call, after the popping statement is validated.
             */
            void popBreakCatcher();
            /*!
             \brief Removes the last statement held by the continue stack. This method should always follow a \c pushContinueCatcher() call, after the popping statement is validated.
             */
            void popContinueCatcher();
            /*!
             \brief Removes the last statement held by the case stack. This method should always follow a \c pushCaseCatcher() call, after the popping statement is validated.
             */
            void popCaseCatcher();
            
            /*!
             \brief Returns the first catcher of a \c break statement, or \c nullptr if the control is not in a breakable statement.
             \param target If specified, the innermost statement with the given role is returned.
             */
            ast::Stmt *getFirstBreakCatcher(ast::break_target target = ast::BreakDefault);
            /*!
             \brief Returns the first catcher of a \c continue statement, or \c nullptr if the control is not in a continuable statement.
             \param target If specified, the innermost statement with the given role is returned.
             */
            ast::Stmt *getFirstContinueCatcher(ast::continue_target target = ast::ContinueDefault);
            /*!
             \brief Returns the first catcher of a \c case statement, or \c nullptr if the control is not in a switchable statement.
             \param target If specified, the innermost statement with the given role is returned.
             */
            ast::Stmt *getFirstCaseCatcher();
            
        };
        
    }
}

#endif
