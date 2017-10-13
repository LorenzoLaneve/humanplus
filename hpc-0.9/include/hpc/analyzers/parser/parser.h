//
//  parser.h
//  Human Plus
//
//  Created by Lore on 08/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#ifndef __human_plus_compiler_parser
#define __human_plus_compiler_parser

#include <hpc/analyzers/sources.h>
#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/ast/ast.h>
#include <hpc/ast/builder/builder.h>

#define report_eof() if (lexer->eof()) return false
#define abort_parse() lexer->escape(); return true

namespace hpc {
    namespace parser {
        
        class ParserInstance {
            /*!
             \brief The diagnostics engine this parser instance has to report diagnostics to.
             */
            diag::DiagEngine &diags;
            /*!
             \brief The AST object this parser is building on.
             */
            ast::AbstractSyntaxTree *boundAST;
            /*!
             \brief The wrapper for the module. All of the top-level declarations will be assigned to the bound module wrapper at the moment of parsing.
             */
            modules::ModuleWrapper *boundwrapper = nullptr;
            /*!
             \brief The lexer instance the parser is reading tokens from.
             */
            lexer::LexerInstance *lexer = nullptr;
            /*!
             \brief The AST builder used by this parser.
             */
            ast::Builder builder;

        public:
            /*!
             \brief Initializes the parser instance with an AST.
             \note If a \c nullptr is passed as AST, this method will \c assert.
             */
            ParserInstance(diag::DiagEngine &diags, ast::AbstractSyntaxTree *boundAST);
            
            /*!
             \brief Binds an \c ast::CompilationUnit object holding the AST to the parser. The parser will add the parsed components to the bound AST.
             */
            void bindAST(ast::AbstractSyntaxTree *astobj);
            /*!
             \brief Returns the \c ast::CompilationUnit object holding the AST the parser is currently building.
             \note If no AST is bound to the parser, this function will \c assert.
             */
            inline ast::AbstractSyntaxTree *getBoundAST() const {
                return boundAST;
            }
            
            /*!
             \brief Binds the source file with the given pathname to the lexer, and prepares it for reading.
             \return \c true if the file was opened successfully, \c false otherwise.
             */
            bool bindSourceFile(source::SourceFile *inputFile);
            /*!
             \brief Returns the currently bound source file.
             */
            source::SourceFile *getSourceFile();
            /*!
             \brief Unbinds the current source file and closes the lexer.
             */
            void unbindSourceFile();
            /*!
             \brief Returns the module wrapper of the module currently used by the parser.
             \note This method will \c assert if no module is bound at the moment of the call.
             */
            modules::ModuleWrapper *getBoundModuleWrapper();
            
            
            /*!
             \brief Starts parsing the source file currently bound to the lexer.
             */
            void parse();
            
            
            /*!
             \brief Parses the next top level declaration in the lexer's stream, starting from the token in the \c lexer::getCurrentToken() at the moment of the call, and adds it to \c current.
             \param current An AST entity that can contain other entities. The parser will add the top level declaration to this entity. If \c current is \c nullptr, the global scope of the bound AST will be taken.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to this function, and return \c false.
             */
            bool parseTopLevel(ast::NameSpaceDecl *current);
            /*!
             \brief Parses the next namespace declaration in the lexer's stream, starting from the \c namespace token as current token at the moment of the call, and adds it to \c current. Syntax of an Human Plus namespace:
             \code
             namespace <name> {
             <top-levels>
             }
             \endcode
             \param current An AST entity that can contain other entities. The parser will add the top level declaration to this entity. If \c current is \c nullptr, an undefined behavior will occur.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseNameSpace(ast::NameSpaceDecl *current);
            /*!
             \brief Parses the next global declaration in the lexer's stream, starting from the \c let token as current token at the moment of the call, and adds it to \c current. The global declaration that should be found may be a global variable declaration or type alias. Syntax of an Human Plus global declaration:
             \code
             let (<id1, id2, ..., idN> be [[a|an] <type>] [= <initial-value>)+;
             \endcode
             \param current An AST entity that can contain other entities. The parser will add the top level declaration to this entity. If \c current is \c nullptr, an undefined behavior will occur.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseGlobalLetDecl(ast::NameSpaceDecl *current);
            /*!
             \brief Parses the next alias declaration in the lexer's stream, starting from the \c alias token as current token at the moment of the call, and adds it to \c current. The global declaration that should be found may be a global variable declaration or type alias. Syntax of an Human Plus global declaration:
             \code
             alias ((<id1, id2, ..., idN>) = <original-type>)+;
             \endcode
             \param current An AST entity that can contain other entities. The parser will add the top level declaration to this entity. If \c current is \c nullptr, an undefined behavior will occur.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseAliasDecl(ast::NameSpaceDecl *current);
            /*!
             \brief Parses the next function in the lexer's stream, starting from the \c function token or a function attribute token as current token at the moment of the call, and adds it to \c current. Syntax of an Human Plus function:
             \code
             [nostalgic] function <name>(<args>) [[that|which] returns [a|an] <type>] {
             <statements>
             }
             [nostalgic] function <name>(<args>) [[that|which] returns [a|an] <type>];
             \endcode
             \param current An AST entity that can contain other entities. The parser will add the top level function to this entity. If \c current is \c nullptr, an undefined behavior will occur.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseFunction(ast::NameSpaceDecl *current);
            /*!
             \brief Parses the next class in the lexer's stream, starting from the \c class token or a class attribute token as current token at the moment of the call, and adds it to \c current. Syntax of an Human Plus class:
             \code
             class <name> [that|which][ extends <superclass>][ implements <protocol1, protocol2, ..., protocolN>] {
             <class-levels>
             }
             \endcode
             \param current An AST entity that can contain other entities. The parser will add the class to this entity. If \c current is \c nullptr, an undefined behavior will occur.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseClass(ast::NameSpaceDecl *current);
            /*!
             \brief Parses the next protocol in the lexer's stream, starting from the \c protocol token as current token at the moment of the call, and adds it to \c current. Syntax of an Human Plus protocol:
             \code
             protocol <name> [that|which][ extends <superprotocol>] {
             <protocol-levels>
             }
             \endcode
             \param current An AST entity that can contain other entities. The parser will add the protocol to this entity. If \c current is \c nullptr, an undefined behavior will occur.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseProtocol(ast::NameSpaceDecl *current);
            /*!
             \brief Parses the next stated function in the lexer's stream, starting from the first identifier token as current token at the moment of the call, and adds it to \c current. Examples of an Human Plus stated function:
             \code
             sine of (an integer p) (function which returns an integer) {
             ...
             }
             
             send (a string s) to (a person p) {
             ...
             }
             \endcode
             \param current An AST entity that can contain other entities. The parser will add the stated function to this entity. If \c current is \c nullptr, an undefined behavior will occur.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseStatedFunction(ast::NameSpaceDecl *current);
            
            
            /*!
             \brief Parses the next class field declaration in the lexer's stream, starting from the \c let token as current token at the moment of the call, and adds it to \c current. The class field that should be found may be a class field declaration or type alias. Syntax of an Human Plus class field:
             \code
             let (<id1, id2, ..., idN> be [[a|an] <type>] [= <initial-value>)+;
             \endcode
             \param current An AST entity that can contain other entities. The parser will add the top level declaration to this entity. If \c current is \c nullptr, an undefined behavior will occur.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseFieldDeclaration(ast::ClassDecl *current);
            /*!
             \brief Parses the next class member declaration in the lexer's stream and adds it to \c current.
             \param current An AST object describing an Human Plus class. The parser will add the top level declaration to this entity. If \c current is \c nullptr, an undefined behavior will occur.
             */
            bool parseClassMember(ast::ClassDecl *current);
            
            
            /*!
             \brief Parses the next type qualifiers in the lexer's stream, starting from the token in the \c lexer::getCurrentToken() at the moment of the call.
             \param report A boolean indicating whether the parser should report an error if the lexer found a non-type sequence.
             \return A pointer to an \c ast::TypeQualifiers bitfield describing the parsed type, or \c nullptr if a syntax error occurred.
             \note This function already reports the syntax error (if \c report is \c true ) before returning \c nullptr. You <b>should not</b> report any error for the \c nullptr returned.
             */
            ast::TypeQualifiers *parseQualifiers(bool report = true);
            /*!
             \brief Parses the next type in the lexer's stream, starting from the token in the \c lexer::getCurrentToken() at the moment of the call.
             \param report A boolean indicating whether the parser should report an error if the lexer found a non-type sequence.
             \return A pointer to an \c ast::Type object describing the parsed type, or \c nullptr if a syntax error occurred.
             \note This function already reports the syntax error (if \c report is \c true ) before returning \c nullptr. You <b>should not</b> report any error for the \c nullptr returned.
             */
            ast::Type *parseType(bool report = true);
            
            
            /*!
             \brief Parses the next statement in the lexer's stream, starting from the first token which composes the statement as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function. A \c nullptr to this function will lead to an undefined behavior.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next statement in the lexer's stream, starting from the first token which composes the statement as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseSubStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next complementary statement in the lexer's stream, starting from the first token which composes the complement as current token at the moment of the call, and puts the pointer into \c parsing.
             \param statement A reference to the pointer to an \c ast::Stmt object. If there is a valid complementary statement without syntax errors, the statement passed to this object will be replaced by an enclosing statement corresponding to the complementary statement.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseComplement(ast::Stmt *&statement);
            /*!
             \brief Parses the next statement in the lexer's stream, starting from the opening token '{' as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::CompoundStmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseCompoundStatement(ast::CompoundStmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next statement in the lexer's stream, starting from the opening token '{' as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseCompoundStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next \c let declaration statement in the lexer's stream, starting from the \c let token as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseVarDeclarationStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next if structure statement in the lexer's stream, starting from the \c if token as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseIfStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next switch structure statement in the lexer's stream, starting from the \c switch token as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseSwitchStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next \c while or \c until iteration statement in the lexer's stream, starting from the \c do token as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parsePreConditionedIterationStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next \c do-while or \c do-until iteration statement in the lexer's stream, starting from the \c do token as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parsePostConditionedIterationStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next \c for declaration statement in the lexer's stream, starting from the \c for token as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseForIterationStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next \c return statement in the lexer's stream, starting from the \c return token as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseReturnStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next \c break statement in the lexer's stream, starting from the \c break token as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseBreakStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            /*!
             \brief Parses the next \c continue statement in the lexer's stream, starting from the \c continue token as current token at the moment of the call, and puts the pointer into \c parsing.
             \param parsing A reference to the pointer to an \c ast::Stmt object. The pointer will be set to the pointer to an object describing the parsed statement.
             \param containerFunc A pointer to a \c ast::FunctionDecl describing the function to which the parsed statement should be added. The function <b>will not</b> add the statement to the function.
             \return \c false if an \c EOF is found during the parsing process, \c true otherwise.
             \note <b>Always</b> control whether this method returned \c false, or if \c EOF is still unexpected after this entity, just control if \c lexer::eof(). As to avoid the control to be stuck on an infinite iteration, the parser control <b>must</b> immediately return to \c lexer::parseTopLevel(), and return \c false.
             */
            bool parseContinueStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc);
            
            
            /*!
             \brief Parses the next symbol in the lexer's stream, starting from the token in the \c lexer::getCurrentToken() at the moment of the call. A symbol is an array of identifiers in a C++ style ( \c this::is::a::symbol )
             \return An \c ast::Symbol object describing the parsed symbol, or an invalid symbol ( \c !isValid() ) if a syntax error occurred.
             \note This function already reports the syntax error before returning the invalid symbol. You <b>should not</b> report any error for the invalid symbol returned.
             */
            ast::Symbol parseSymbol();
            /*!
             \brief Parses the next expression in the lexer's stream, starting from the token in the \c lexer::getCurrentToken() at the moment of the call.
             \return A pointer to an \c ast::Expr object describing the parsed expression, or \c nullptr if a syntax error occurred.
             \note This function already reports the syntax error before returning \c nullptr. You <b>should not</b> report any error for the \c nullptr returned.
             */
            ast::Expr *parseExpression();

            /// Subfunctions for parseExpression() DON'T USE
        private:
            ast::Expr *parseMemberAccessExpression(ast::Expr *lhs);
            
            ast::Expr *parseHandSideExpression();
            
            ast::Expr *parseRightHandSideExpression(lexer::token_ty exprPrecedence, ast::Expr *lhs);
            
        public:
            
        };

    }
}

#endif
