// => src/analyzers/parser/expressions.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/parser/parser.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/analyzers/lexer/lexer.h>
#include <hpc/analyzers/sources.h>
#include <hpc/analyzers/syntax/operators.h>
#include <hpc/ast/exprs/binary.h>
#include <hpc/ast/exprs/unary.h>
#include <hpc/ast/exprs/constant.h>
#include <hpc/ast/exprs/reference.h>
#include <hpc/ast/exprs/members.h>

using namespace hpc;

ast::Symbol parser::ParserInstance::parseSymbol() {
    source::TokenRef lastidref;
    lexer->getCurrentToken(&lastidref);
    ast::Symbol parsingsym(lexer->getCurrentIdentifier(), &lastidref);
    
    while (lexer->getNextToken() == lexer::TokenNameSpaceBrowser) {
        if (lexer->getNextToken(&lastidref) == lexer::TokenIdentifier) {
            parsingsym.pushBackChild(lexer->getCurrentIdentifier(), &lastidref);
        } else {
            if (!lexer->eof()) diags.reportError(diag::ExpectedUnqualifiedIdentifier, &lastidref);
            return ast::Symbol();
        }
    }
    
    return parsingsym;
}

ast::Expr *parser::ParserInstance::parseMemberAccessExpression(ast::Expr *lhs) {
    if (!lhs) return nullptr;
    
    source::TokenRef operref;
    if (lexer->getCurrentToken(&operref) != lexer::TokenOperatorMemberAccess) {
        return lhs;
    }
    
    source::TokenRef memberidref;
    if (lexer->getNextToken(&memberidref) != lexer::TokenIdentifier) {
        diags.reportError(diag::ExpectedMemberIdentifier, &memberidref);
        return nullptr;
    }
    
    lhs = new ast::FieldRef(lhs, lexer->getCurrentIdentifier());
    lhs->tokenRef(ast::PointToStructAccessOperator, operref);
    
    lexer->getNextToken();
    return parseMemberAccessExpression(lhs);
}

ast::Expr *parser::ParserInstance::parseHandSideExpression() {

    if (syntax::isUnaryOperator(lexer->getCurrentToken())) {
        lexer::token_ty unaryoper = lexer->getCurrentToken();
        lexer->getNextToken();
    
        switch (unaryoper) {
            case lexer::TokenOperatorMinus:
                return new ast::ArithmeticNegationExpr(parseHandSideExpression());
            case lexer::TokenOperatorExclMark:
                return new ast::LogicalNegationExpr(parseHandSideExpression());
            case lexer::TokenOperatorTilde:
                return new ast::BitwiseNegationExpr(parseHandSideExpression());
        }
    }
    
    ast::Expr *hs = nullptr;
    
    source::TokenRef exprbeginref;
    switch (lexer->getCurrentToken(&exprbeginref)) {
        case lexer::TokenNull:
            hs = new ast::NullPointer(ast::BuiltinType::get(ast::BuiltinType::SignedInteger));
            break;
        case lexer::TokenCharacterLiteral:
            hs = new ast::CharLiteral(lexer->currentCharacter);
            break;
        case lexer::TokenIntegerLiteral:
            hs = new ast::IntegerLiteral(lexer->currentInteger);
            break;
        case lexer::TokenUnsignedIntegerLiteral:
            hs = new ast::UIntegerLiteral(lexer->currentUnsignedInteger);
            break;
        case lexer::TokenLongLiteral:
            hs = new ast::LongLiteral(lexer->currentLong);
            break;
        case lexer::TokenUnsignedLongLiteral:
            hs = new ast::ULongLiteral(lexer->currentUnsignedLong);
            break;
        case lexer::TokenFloatLiteral:
            hs = new ast::FloatLiteral(lexer->currentFloat);
            break;
        case lexer::TokenDoubleLiteral:
            hs = new ast::DoubleLiteral(lexer->currentDouble);
            break;
        case lexer::TokenTrue:
            hs = new ast::BoolLiteral(true);
            break;
        case lexer::TokenFalse:
            hs = new ast::BoolLiteral(false);
            break;
        case lexer::TokenStringLiteral:
            hs = new ast::StringLiteral(lexer->currentString);
            break;
        case lexer::TokenIdentifier: {
            ast::Symbol callsymbol = parseSymbol();
            
            source::TokenRef topidref;
            lexer->getLastToken(&topidref);
            
            if (lexer->getCurrentToken() != '(') {
                ast::VarRef *nvarref = new ast::VarRef(callsymbol);
                nvarref->tokenRef(ast::PointToBeginOfExpression, exprbeginref);
                nvarref->tokenRef(ast::PointToEndOfExpression, topidref);
                
                return parseMemberAccessExpression(nvarref);
            }
            
            ast::FunctionCall *fcall = new ast::FunctionCall(callsymbol);
            lexer->getNextToken();
            
            source::TokenRef lastexprref;
            while (lexer->getCurrentToken(&lastexprref) != ')') {
                ast::Expr *argexpr = parseExpression();
                if (!argexpr) {
                    if (!lexer->eof()) diags.reportError(diag::ExpectedExpression, &lastexprref);
                    return nullptr;
                }
                
                fcall->addCallParameter(argexpr);
                
                source::TokenRef commaref;
                if (lexer->getCurrentToken(&commaref) != ',' && lexer->getCurrentToken() != ')') {
                    if (!lexer->eof()) diags.reportError(diag::InvalidArgumentList, &commaref);
                    return nullptr;
                } else if (lexer->getCurrentToken() != ')') lexer->getNextToken();
            }
            
            hs = fcall;
            break;
        }
        case '(': {
            lexer->getNextToken();
            hs = parseExpression();
            
            source::TokenRef closeref;
            if (lexer->getCurrentToken(&closeref) != ')') {
                if (!lexer->eof()) diags.reportError(diag::ExpectedClosedTuple, &closeref);
                return nullptr;
            }
            break;
        }
        default: {
            if (!lexer->eof()) diags.reportError(diag::ExpectedExpression, &exprbeginref);
            return nullptr;
        }
    }
    
    if (hs) {
        hs->tokenRef(ast::PointToBeginOfExpression, exprbeginref);
        
        source::TokenRef exprendref;
        lexer->getCurrentToken(&exprendref);
        hs->tokenRef(ast::PointToEndOfExpression, exprendref);
    }
    
    lexer->getNextToken();
    return parseMemberAccessExpression(hs);
}

ast::Expr *parser::ParserInstance::parseRightHandSideExpression(lexer::token_ty exprPrecedence, ast::Expr *lhs) {
    source::TokenRef operref;
    while (1) {
        int currentPrecedence = syntax::getOperatorPrecedence(lexer->getCurrentToken());
        
        if (currentPrecedence < exprPrecedence) return lhs;
        
        lexer::token_ty binaryOperator = lexer->getCurrentToken(&operref);
        
        if (!syntax::isBinaryOperator(binaryOperator)) return lhs;
        lexer->getNextToken();
    
        ast::Expr *rhs = parseHandSideExpression();
        if (!rhs) return nullptr;
            
        int nextPrecedence = syntax::getOperatorPrecedence(lexer->getCurrentToken());
        if (currentPrecedence < nextPrecedence || (nextPrecedence >= 0 && binaryOperator == lexer::TokenOperatorAssign)) {
            rhs = parseRightHandSideExpression(currentPrecedence+1, rhs);
            if (!rhs) return nullptr;
        }
        
        lhs = ast::BinaryExpr::create(lhs, binaryOperator, rhs);
        lhs->tokenRef(ast::PointToOperator, operref);
    }
}

ast::Expr *parser::ParserInstance::parseExpression() {
    source::TokenRef exprbeginref, exprendref;
    lexer->getCurrentToken(&exprbeginref);
    
    ast::Expr *lhs = parseHandSideExpression();
    if (!lhs) return nullptr;
    
    ast::Expr *binexpr = parseRightHandSideExpression(0, lhs);
    if (!binexpr) return nullptr;
    
    lexer->getLastToken(&exprendref);
    
    binexpr->tokenRef(ast::PointToBeginOfExpression, exprbeginref);
    binexpr->tokenRef(ast::PointToEndOfExpression, exprendref);
    
    return binexpr;
}

