//
//  classes.cpp
//  HumanPlus
//
//  Created by Lore on 01/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/analyzers/parser/parser.h>
#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/analyzers/sources.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/symbols.h>
#include <hpc/ast/decls/class.h>

using namespace hpc;

bool parser::ParserInstance::parseFieldDeclaration(ast::ClassDecl *current) {
    source::TokenRef lastidref;
    if (lexer->getNextToken(&lastidref) == lexer::TokenIdentifier) {
        while (1) {
            std::vector<ast::SymbolIdentifier> names;
            while (1) {
                names.push_back({ lexer->getCurrentIdentifier(), new source::TokenRef(lastidref) });
                
                if (lexer->getNextToken() == ',') {
                    if (lexer->getNextToken(&lastidref) != lexer::TokenIdentifier) {
                        report_eof();
                        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &lastidref);
                        abort_parse();
                    }
                } else break;
            }
            report_eof();
            
            source::TokenRef betkref;
            if (lexer->getCurrentToken(&betkref) != lexer::TokenBe) {
                report_eof();
                diags.reportError(diag::ExpectedTokenBeAfterLetDeclaration, &betkref);
                abort_parse();
            } else lexer->getNextToken();
            
            lexer->ignoreArticleIfAny();
            report_eof();
            
            if (lexer->getCurrentToken() == lexer::TokenAlias) {
                source::TokenRef oftkref;
                if (lexer->getNextToken(&oftkref) == lexer::TokenIdentifier && lexer->getCurrentIdentifier() == "of") {
                    lexer->getNextToken();
                    ast::Type *original = parseType();
                    if (!original) { abort_parse(); }
                    report_eof();
                    
                    for (ast::SymbolIdentifier &taid : names) {
                        ast::TypeAliasDecl *newtyalias = new ast::TypeAliasDecl(taid.identifier, original, current);
                        if (taid.symref) newtyalias->tokenRef(ast::PointToVariableIdentifier, *taid.symref);
                        
                        current->addTypeAlias(newtyalias);
                    }
                } else {
                    report_eof();
                    diags.reportError(diag::ExpectedOfAfterAlias, &oftkref);
                    abort_parse();
                }
            } else {
                report_eof();
                
                ast::Type *type = nullptr;
                ast::Expr *initval = nullptr;
                
                if (lexer->getCurrentToken() == '=') {
                    lexer->getNextToken();
                    initval = parseExpression();
                    
                    if (!initval) {
                        report_eof();
                        abort_parse();
                    }
                } else {
                    
                    type = parseType();
                    if (!type) { abort_parse(); }
                    report_eof();
                    
                    if (lexer->getCurrentToken() == '=') {
                        lexer->getNextToken();
                        initval = parseExpression();
                        
                        if (!initval) {
                            report_eof();
                            abort_parse();
                        }
                    }
                }
                
                for (ast::SymbolIdentifier &gvid : names) {
                    ast::FieldDecl *newgvar = new ast::FieldDecl(gvid.identifier, type, current, initval);
                    if (gvid.symref) newgvar->tokenRef(ast::PointToVariableIdentifier, *gvid.symref);
                    
                    current->addField(newgvar);
                }
            }
            
            source::TokenRef delimref;
            lexer->getLastToken(&delimref);
            delimref = delimref.getNextPoint();
            if (lexer->getCurrentToken() == ',') {
                source::TokenRef newvarref;
                if (lexer->getNextToken(&newvarref) != lexer::TokenIdentifier) {
                    report_eof();
                    diags.reportError(diag::ExpectedUnqualifiedIdentifier, &newvarref);
                    abort_parse();
                }
            } else if (!lexer::isDelimiter(lexer->getCurrentToken())) {
                report_eof();
                diags.reportError(diag::ExpectedDelimiterAfterTopLevel, &delimref);
                abort_parse();
            } else break;
        }
        
        lexer->getNextToken();
    } else {
        report_eof();
        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &lastidref);
        abort_parse();
    }
    return true;
}

bool parser::ParserInstance::parseClassMember(ast::ClassDecl *current) {
    
    if (lexer::isDelimiter(lexer->getCurrentToken())) {
        lexer->getNextToken();
    }
    
    switch (lexer->getCurrentToken()) {
        case lexer::TokenLet:
            return parseFieldDeclaration(current);
    }
    
    return true;
}
