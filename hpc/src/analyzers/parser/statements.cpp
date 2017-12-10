// => src/analyzers/parser/statements.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/parser/parser.h>
#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/analyzers/sources.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/stmt/ifelse.h>
#include <hpc/ast/stmt/whileuntil.h>
#include <hpc/ast/stmt/for.h>

using namespace hpc;

bool parser::ParserInstance::parseStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    return parseSubStatement(parsing, containerFunc);
}

bool parser::ParserInstance::parseComplement(ast::Stmt *&statement) {
    switch (lexer->getCurrentToken()) {
        case lexer::TokenIf: {
            lexer->getNextToken();
            ast::Expr *condition = parseExpression();
            
            if (!condition) {
                report_eof();
                abort_parse();
            } else statement = new ast::IfStmt(condition, statement);
            break;
        }
        case lexer::TokenWhile: {
            lexer->getNextToken();
            ast::Expr *condition = parseExpression();
            
            if (!condition) {
                report_eof();
                abort_parse();
            } else statement = new ast::PreWhileStmt(condition, statement);
            break;
        }
        case lexer::TokenUntil: {
            lexer->getNextToken();
            ast::Expr *condition = parseExpression();
            
            if (!condition) {
                report_eof();
                abort_parse();
            } else statement = new ast::PreUntilStmt(condition, statement);
            break;
        }
    }
    
    return true;
}

bool parser::ParserInstance::parseVarDeclarationStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    source::SrcLoc lastidref;
    if (lexer->getNextToken(&lastidref) == lexer::TokenIdentifier) {
        ast::VarDeclStmt *declaration = new ast::VarDeclStmt(containerFunc);
        
        while (1) {
            std::vector<ast::SymbolIdentifier> names;
            
            while (1) {
                names.push_back({ lexer->getCurrentIdentifier(), new source::SrcLoc(lastidref) });
                
                if (lexer->getNextToken() == ',') {
                    if (lexer->getNextToken(&lastidref) != lexer::TokenIdentifier) {
                        report_eof();
                        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &lastidref);
                        abort_parse();
                        break;
                    }
                } else {
                    report_eof();
                    break;
                }
            }
            
            source::SrcLoc betkref;
            if (lexer->getCurrentToken(&betkref) != lexer::TokenBe) {
                report_eof();
                diags.reportError(diag::ExpectedTokenBeAfterLetDeclaration, &betkref);
                abort_parse();
            }
            else lexer->getNextToken();
            
            
            ast::Type *type = nullptr;
            ast::Expr *initval = nullptr;
            source::SrcLoc equalref;
            if (lexer->getCurrentToken(&equalref) == '=') {
                lexer->getNextToken();
                initval = parseExpression();
                if (!initval) {
                    report_eof();
                    abort_parse();
                }
            } else {
                lexer->ignoreArticleIfAny();
                
                type = parseType();
                report_eof();
                
                if (lexer->getCurrentToken(&equalref) == '=') {
                    lexer->getNextToken();
                    initval = parseExpression();
                    if (!initval) {
                        report_eof();
                        abort_parse();
                    }
                }
            }
            
            //if (names.size() > 1) {
            //    VS.diags.reportError(diag::InitValueForMoreThanOneVariable, &equalref);
            //}
        
            for (ast::SymbolIdentifier &lvid : names) {
                ast::LocalVar *newlocvar = new ast::LocalVar(lvid.identifier, type, initval);
                if (lvid.symref) newlocvar->tokenRef(ast::PointToVariableIdentifier, *lvid.symref);
                
                declaration->addVariable(newlocvar);
            }
            
            source::SrcLoc delimref;
            lexer->getLastToken(&delimref);
            delimref = delimref.getNextPoint();
            if (lexer->getCurrentToken() == ',') {
                if (lexer->getNextToken(&lastidref) != lexer::TokenIdentifier) {
                    report_eof();
                    diags.reportError(diag::ExpectedUnqualifiedIdentifier, &lastidref);
                    abort_parse();
                    break;
                }
            } else if (!lexer::isDelimiter(lexer->getCurrentToken())) {
                report_eof();
                diags.reportError(diag::ExpectedDelimiterAfterLocalLet, &delimref);
                abort_parse();
                break;
            } else break;
        }
        
        lexer->getNextToken();
        parsing = declaration;
    } else {
        report_eof();
        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &lastidref);
        abort_parse();
    }
    return true;
}

bool parser::ParserInstance::parseIfStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    lexer->getNextToken();
    ast::Expr *condition = parseExpression();
    
    if (!condition) {
        report_eof();
        abort_parse();
    }
    
    if (lexer->getCurrentToken() == lexer::TokenThen) lexer->getNextToken();
    ast::Stmt *thenBlock = nullptr, *elseBlock = nullptr;
    parseStatement(thenBlock, containerFunc);
    
    report_eof();
    if (lexer->getCurrentToken() == lexer::TokenElse) {
        lexer->getNextToken();
        parseStatement(elseBlock, containerFunc);
    }
    
    report_eof();
    parsing = new ast::IfStmt(condition, thenBlock, elseBlock);
    return true;
}

bool parser::ParserInstance::parseSwitchStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    lexer->getNextToken();
    ast::Expr *switchingVal = parseExpression();
    
    if (!switchingVal) {
        report_eof();
        abort_parse();
    }
    
    // CONTINUE HERE
    
    return true;
}

bool parser::ParserInstance::parsePreConditionedIterationStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    lexer::token_ty itertype = lexer->getCurrentToken();
    lexer->getNextToken();
    ast::Expr *condition = parseExpression();
    if (!condition) {
        report_eof();
        abort_parse();
    }
    
    ast::Stmt *block = nullptr;
    parseStatement(block, containerFunc);
    report_eof();
    
    if (itertype == lexer::TokenWhile)
        parsing = new ast::PreWhileStmt(condition, block);
    else parsing = new ast::PreUntilStmt(condition, block);
    return true;
}

bool parser::ParserInstance::parsePostConditionedIterationStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    lexer->getNextToken();
    
    ast::Stmt *block = nullptr;
    parseStatement(block, containerFunc);
    report_eof();
    
    lexer::token_ty itertype = 0;
    
    source::SrcLoc whileuntilref;
    switch (lexer->getCurrentToken(&whileuntilref)) {
        case lexer::TokenWhile:
        case lexer::TokenUntil:
            itertype = lexer->getCurrentToken();
            break;
        default:
            report_eof();
            diags.reportError(diag::ExpectedWhileOrUntilAfterDo, &whileuntilref);
            abort_parse();
    }
    
    lexer->getNextToken();
    ast::Expr *condition = parseExpression();
    if (!condition) {
        report_eof();
        abort_parse();
    }
    
    if (itertype == lexer::TokenWhile)
        parsing = new ast::PostWhileStmt(condition, block);
    else parsing = new ast::PostUntilStmt(condition, block);

    source::SrcLoc delimref;
    lexer->getLastToken(&delimref);
    if (!lexer::isDelimiter(lexer->getCurrentToken())) {
        report_eof();
        if (itertype == lexer::TokenWhile)
            diags.reportError(diag::ExpectedDelimiterAfterDoWhile, &delimref);
        else diags.reportError(diag::ExpectedDelimiterAfterDoUntil, &delimref);
        return true;
    }

    lexer->getNextToken();
    return true;
}

bool parser::ParserInstance::parseForIterationStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    lexer->getNextToken();
    
    bool hasTuple = false;
    if (lexer->getCurrentToken() == '(') {
        hasTuple = true;
        lexer->getNextToken();
    }
    
    std::vector<ast::Stmt *> initstmt;
    
    if (lexer->getCurrentToken() == lexer::TokenLet) {
        ast::Stmt *indexdecl = nullptr;
        parseVarDeclarationStatement(indexdecl, containerFunc);
        
        initstmt.push_back(indexdecl);
    } else {
        do {
            ast::Expr *initst = parseExpression();
            if (!initst) {
                report_eof();
                abort_parse();
            }
            initstmt.push_back(initst);
        } while (lexer->getCurrentToken() == ',' && lexer->getNextToken());
        
        if (!lexer::isDelimiter(lexer->getCurrentToken())) {
            report_eof();
            abort_parse();
        }
        
        lexer->getNextToken();
    }
    
    ast::Expr *condition = parseExpression();
    if (!condition) {
        report_eof();
        abort_parse();
    }
    
    if (!lexer::isDelimiter(lexer->getCurrentToken())) {
        report_eof();
        abort_parse();
    }
    
    std::vector<ast::Stmt *> endstmt;
    do {
        lexer->getNextToken();
        ast::Expr *endst = parseExpression();
        if (!endst) {
            report_eof();
            abort_parse();
        }
        endstmt.push_back(endst);
    } while (lexer->getCurrentToken() == ',');
    
    source::SrcLoc cltupleref;
    if (hasTuple && lexer->getCurrentToken(&cltupleref) != ')') {
        report_eof();
        diags.reportError(diag::ExpectedClosedTuple, &cltupleref);
        abort_parse();
    } else if (hasTuple)
        lexer->getNextToken();
    
    ast::Stmt *block = nullptr;
    parseStatement(block, containerFunc);
    report_eof();
    
    parsing = new ast::ForStmt(initstmt, condition, endstmt, block);
    return true;
}

bool parser::ParserInstance::parseReturnStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    source::SrcLoc qualifref;
    lexer->getCurrentToken(&qualifref);
    
    lexer->getNextToken();
    if (!lexer::isDelimiter(lexer->getCurrentToken())) {
        report_eof();
        
        ast::Expr *retval = parseExpression();
        
        if (!retval) {
            report_eof();
            abort_parse();
        }
        
        parsing = new ast::ReturnStmt(containerFunc, retval);
    } else parsing = new ast::ReturnStmt(containerFunc, nullptr);
    parsing->tokenRef(ast::PointToStatementQualifier, qualifref);
    
    parseComplement(parsing);
    
    source::SrcLoc delimref;
    lexer->getLastToken(&delimref);
    delimref = delimref.getNextPoint();
    if (!lexer::isDelimiter(lexer->getCurrentToken())) {
        report_eof();
        diags.reportError(diag::ExpectedDelimiterAfterReturn, &delimref);
        abort_parse();
    }
    lexer->getNextToken();
    return true;
}

bool parser::ParserInstance::parseBreakStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    source::SrcLoc qualifref;
    lexer->getCurrentToken(&qualifref);
    lexer->getNextToken();
    
    parsing = new ast::BreakStmt();
    parsing->tokenRef(ast::PointToStatementQualifier, qualifref);
    parseComplement(parsing);
    
    source::SrcLoc delimref;
    lexer->getLastToken(&delimref);
    delimref = delimref.getNextPoint();
    if (!lexer::isDelimiter(lexer->getCurrentToken())) {
        report_eof();
        diags.reportError(diag::ExpectedDelimiterAfterBreak, &delimref);
        abort_parse();
    }
    lexer->getNextToken();
    return true;
}

bool parser::ParserInstance::parseContinueStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    source::SrcLoc qualifref;
    lexer->getCurrentToken(&qualifref);
    lexer->getNextToken();
    
    parsing = new ast::ContinueStmt();
    parsing->tokenRef(ast::PointToStatementQualifier, qualifref);
    parseComplement(parsing);
    
    source::SrcLoc delimref;
    lexer->getLastToken(&delimref);
    delimref = delimref.getNextPoint();
    if (!lexer::isDelimiter(lexer->getCurrentToken())) {
        report_eof();
        diags.reportError(diag::ExpectedDelimiterAfterContinue, &delimref);
        abort_parse();
    }
    lexer->getNextToken();
    return true;
}

bool parser::ParserInstance::parseSubStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    switch (lexer->getCurrentToken()) {
        case lexer::TokenLet:
            return parseVarDeclarationStatement(parsing, containerFunc);
        case lexer::TokenIf:
            return parseIfStatement(parsing, containerFunc);
        case lexer::TokenSwitch:
            return parseSwitchStatement(parsing, containerFunc);
        case lexer::TokenWhile:
        case lexer::TokenUntil:
            return parsePreConditionedIterationStatement(parsing, containerFunc);
        case lexer::TokenDo:
            return parsePostConditionedIterationStatement(parsing, containerFunc);
        case lexer::TokenFor:
            return parseForIterationStatement(parsing, containerFunc);
        case lexer::TokenReturnStatement:
            return parseReturnStatement(parsing, containerFunc);
        case lexer::TokenBreak:
            return parseBreakStatement(parsing, containerFunc);
        case lexer::TokenContinue:
            return parseContinueStatement(parsing, containerFunc);
        case '{':
            return parseCompoundStatement(parsing, containerFunc);
        default:
            if (lexer::isDelimiter(lexer->getCurrentToken())) {
                lexer->getNextToken();
                parsing = nullptr;
            } else {
                parsing = parseExpression();
                if (!parsing) {
                    report_eof();
                    abort_parse();
                }
                
                parseComplement(parsing);
                
                source::SrcLoc delimref;
                lexer->getLastToken(&delimref);
                delimref = delimref.getNextPoint();
                if (!lexer::isDelimiter(lexer->getCurrentToken())) {
                    report_eof();
                    diags.reportError(diag::ExpectedDelimiterAfterExpression, &delimref);
                    return true;
                }
                
                lexer->getNextToken();
            }
    }
    
    return true;
}

bool parser::ParserInstance::parseCompoundStatement(ast::CompoundStmt *&parsing, ast::FunctionDecl *containerFunc) {
    return parseCompoundStatement((ast::Stmt *&)parsing, containerFunc);
}

bool parser::ParserInstance::parseCompoundStatement(ast::Stmt *&parsing, ast::FunctionDecl *containerFunc) {
    ast::CompoundStmt *cmpstmt = new ast::CompoundStmt();
    
    lexer->getNextToken();
    
    while (lexer->getCurrentToken() != '}') {
        report_eof();
        
        ast::Stmt *statement = nullptr;
        if(!parseSubStatement(statement, containerFunc)) return false;
        
        if (statement) cmpstmt->pushSubStatement(statement);
    }
    
    source::SrcLoc endref;
    lexer->getCurrentToken(&endref);
    cmpstmt->tokenRef(ast::PointToEndOfCompoundStatement, endref);
    
    lexer->getNextToken();
    parsing = cmpstmt;
    return true;
}

