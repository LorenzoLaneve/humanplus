// => src/analyzers/parser/toplevel.cpp
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
#include <hpc/ast/types/base.h>
#include <hpc/ast/decls/namespace.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/decls/function.h>
#include <hpc/ast/decls/class.h>
#include <hpc/ast/decls/protocol.h>

#include <string>

using namespace hpc;

bool parser::ParserInstance::parseNameSpace(ast::NameSpaceDecl *current) {
    src::SrcLoc idref;
    if (lexer->getNextToken(&idref) == lexer::TokenIdentifier) {
        ast::NameSpaceDecl *lib = builder.getOrCreateNameSpace(lexer->getCurrentIdentifier());
        
        src::SrcLoc openbref;
        if (lexer->getNextToken(&openbref) == '{') {
            lexer->getNextToken();
            
            while (lexer->getCurrentToken() != '}') {
                report_eof();
                parseTopLevel(lib);
            }
            
            builder.setInsertNameSpace(current);
            lexer->getNextToken();
        } else {
            report_eof();
            diags.reportError(diag::ExpectedOpenBrace, &openbref);
            abort_parse();
        }
    } else {
        report_eof();
        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &idref);
        abort_parse();
    }
    report_eof();
    return true;
}

bool parser::ParserInstance::parseGlobalLetDecl(ast::NameSpaceDecl *current) {
    src::SrcLoc lastidref;
    if (lexer->getNextToken(&lastidref) == lexer::TokenIdentifier) {
        while (1) {
            std::vector<ast::SymbolIdentifier> names;
            while (1) {
                names.push_back({ lexer->getCurrentIdentifier(), new src::SrcLoc(lastidref) });
                
                if (lexer->getNextToken() == ',') {
                    if (lexer->getNextToken(&lastidref) != lexer::TokenIdentifier) {
                        report_eof();
                        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &lastidref);
                        abort_parse();
                    }
                } else break;
            }
            report_eof();
            
            src::SrcLoc betkref;
            if (lexer->getCurrentToken(&betkref) != lexer::TokenBe) {
                report_eof();
                diags.reportError(diag::ExpectedTokenBeAfterLetDeclaration, &betkref);
                abort_parse();
            } else lexer->getNextToken();
            
            lexer->ignoreArticleIfAny();
            report_eof();
            
            if (lexer->getCurrentToken() != lexer::TokenAlias) { // normal variable declaration
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
                    ast::GlobalVar *newgvar = builder.createGlobalVariable(gvid.identifier, type, initval);
                    boundwrapper->addDeclaration(newgvar);
                    
                    if (gvid.symref)
                        newgvar->tokenRef(ast::PointToVariableIdentifier, *gvid.symref);
                }
            } else { // old alias declaration
                src::SrcLoc oftkref;
                if (lexer->getNextToken(&oftkref) == lexer::TokenIdentifier && lexer->getCurrentIdentifier() == "of") {
                    lexer->getNextToken();
                    ast::Type *original = parseType();
                    if (!original) { abort_parse(); }
                    report_eof();
                    
                    for (ast::SymbolIdentifier &taid : names) {
                        ast::TypeAliasDecl *newtyalias = builder.createTypeAliasDecl(taid.identifier, original);
                        if (taid.symref)
                            newtyalias->tokenRef(ast::PointToVariableIdentifier, *taid.symref);
                    }
                } else {
                    report_eof();
                    diags.reportError(diag::ExpectedOfAfterAlias, &oftkref);
                    abort_parse();
                }
            }
            
            src::SrcLoc delimref;
            lexer->getLastToken(&delimref);
            delimref = delimref.getNextPoint();
            if (lexer->getCurrentToken() == ',') {
                src::SrcLoc newvarref;
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

bool parser::ParserInstance::parseAliasDecl(ast::NameSpaceDecl *current) {
    src::SrcLoc lastidref;
    if (lexer->getNextToken(&lastidref) == lexer::TokenIdentifier) {
        while (1) {
            std::vector<ast::SymbolIdentifier> names;
            while (1) {
                names.push_back({ lexer->getCurrentIdentifier(), new src::SrcLoc(lastidref) });
                
                if (lexer->getNextToken() == ',') {
                    if (lexer->getNextToken(&lastidref) != lexer::TokenIdentifier) {
                        report_eof();
                        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &lastidref);
                        abort_parse();
                    }
                } else break;
            }
            report_eof();
            
            src::SrcLoc eqtkref;
            if (lexer->getCurrentToken(&eqtkref) == '=' /*|| (lexer->getCurrentToken() == lexer::TokenIdentifier && lexer->getCurrentIdentifier() == "of")*/) {
                // FIXME "of" as token and lexer contexts
                lexer->getNextToken();
                ast::Type *original = parseType();
                if (!original) { abort_parse(); }
                report_eof();
                
                for (ast::SymbolIdentifier &typeAliasID : names) {
                    ast::TypeAliasDecl *newtyalias = builder.createTypeAliasDecl(typeAliasID.identifier, original);
                    if (typeAliasID.symref)
                        newtyalias->tokenRef(ast::PointToVariableIdentifier, *typeAliasID.symref);
                }
            } else {
                report_eof();
                diags.reportError(diag::ExpectedEqualAfterAlias, &lastidref);
                abort_parse();
            }
            
            src::SrcLoc delimref;
            lexer->getLastToken(&delimref);
            delimref = delimref.getNextPoint();
            if (lexer->getCurrentToken() == ',') {
                src::SrcLoc newvarref;
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

bool parser::ParserInstance::parseFunction(ast::NameSpaceDecl *current) {
    ast::FunctionDecl::FunctionAttributes attributes;
    
    src::SrcLoc funckwref;
    bool finished = false;
    while (!finished) {
        switch (lexer->getCurrentToken()) {
            case lexer::TokenNostalgic:
                attributes.nostalgic = true;
                break;
            case lexer::TokenFunction:
                finished = true;
                break;
            default:
                diags.reportError(diag::ExpectedTokenFunction, &funckwref);
                abort_parse();
        }
        lexer->getNextToken(&funckwref);
        report_eof();
    }
    
    src::SrcLoc funcidref;
    if (lexer->getCurrentToken(&funcidref) == lexer::TokenIdentifier) {
        std::string funcname = lexer->getCurrentIdentifier();
        
        src::SrcLoc argopenref;
        if (lexer->getNextToken(&argopenref) == '(') {
            
            std::vector<ast::ParamVar *> args;
            
            lexer->getNextToken();
            while (lexer->getCurrentToken() != ')') {
                lexer->ignoreArticleIfAny();
                report_eof();
                
                ast::Type *argtype = parseType();
                report_eof();
                if (!argtype) { abort_parse(); }
                
                src::SrcLoc argidref;
                if (lexer->getCurrentToken(&argidref) == lexer::TokenIdentifier) {
                    std::string argname = lexer->getCurrentIdentifier();
                    
                    args.push_back(new ast::ParamVar(argname, argtype, nullptr));
                    
                    src::SrcLoc commaref;
                    if (lexer->getNextToken(&commaref) == ',') {
                        lexer->getNextToken();
                    } else if (lexer->getCurrentToken() != ')') {
                        report_eof();
                        diags.reportError(diag::InvalidArgumentList, &commaref);
                        // FIXME note: open tuple location
                        abort_parse();
                    }
                    
                } else {
                    report_eof();
                    diags.reportError(diag::ExpectedUnqualifiedIdentifier, &argidref);
                    abort_parse();
                }
            }
            
            lexer->getNextToken();
            lexer->ignoreRelativePronounsIfAny();
            
            ast::Type *returntype = nullptr;
            
            if (lexer->getCurrentToken() == lexer::TokenReturnQualifier) {
                lexer->getNextToken();
                lexer->ignoreArticleIfAny();
                returntype = parseType();
                if (!returntype) { abort_parse(); }
                report_eof();
            } else {
                report_eof();
                returntype = ast::BuiltinType::get(ast::BuiltinType::Void);
            }
            
            ast::FunctionDecl *newFunc = builder.createFunctionDecl(funcname, args, returntype, attributes);
            boundwrapper->addDeclaration(newFunc);
            
            newFunc->tokenRef(ast::PointToVariableIdentifier, funcidref);
            
            src::SrcLoc stmtopenref;
            if (lexer->getCurrentToken(&stmtopenref) == '{') {
                ast::CompoundStmt *fblock;
                if (!parseCompoundStatement(fblock, newFunc)) return false;
                
                newFunc->setStatementsBlock(fblock);
            } else if (lexer::isDelimiter(lexer->getCurrentToken())) {
                newFunc->setStatementsBlock(nullptr);
                lexer->getNextToken();
            } else {
                report_eof();
                diags.reportError(diag::ExpectedDelimiterAfterTopLevel, &stmtopenref);
                return true;
            }
        } else {
            report_eof();
            diags.reportError(diag::ExpectedTupleForFunctionDeclaration, &argopenref);
            abort_parse();
        }
    } else {
        report_eof();
        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &funckwref);
        abort_parse();
    }
    return true;
}

bool parser::ParserInstance::parseClass(ast::NameSpaceDecl *current) {
    src::SrcLoc clsnameref;
    if (lexer->getNextToken(&clsnameref) == lexer::TokenIdentifier) {
        std::string classname = lexer->getCurrentIdentifier();
        std::string baseclass = "";
        std::vector<std::string> protocols;
        
        lexer->getNextToken();
        lexer->ignoreRelativePronounsIfAny();
        
        if (lexer->getCurrentToken() == lexer::TokenExtends || lexer->getCurrentToken() == ':') {
            src::SrcLoc superclsref;
            if (lexer->getNextToken(&superclsref) == lexer::TokenIdentifier) {
                baseclass = lexer->getCurrentIdentifier();
                lexer->getNextToken();
            } else {
                report_eof();
                diags.reportError(diag::ExpectedUnqualifiedIdentifier, &superclsref);
                abort_parse();
            }
            
            lexer->ignoreAndIfAny();
        }
        
        lexer->ignoreRelativePronounsIfAny();
        if (lexer->getCurrentToken() == ',' || lexer->getCurrentToken() == lexer::TokenImplements) {
            
            do {
                src::SrcLoc lastidref;
                if (lexer->getNextToken(&lastidref) == lexer::TokenIdentifier)
                    protocols.push_back(lexer->getCurrentIdentifier());
                else {
                    report_eof();
                    diags.reportError(diag::ExpectedUnqualifiedIdentifier, &lastidref);
                    abort_parse();
                    break;
                }
            } while (lexer->getNextToken() == ',');
            
        }
        
        src::SrcLoc clsbodyref;
        if (lexer->getCurrentToken(&clsbodyref) == '{') {
            
            ast::ClassDecl *newclass = builder.createClassDecl(classname);
            newclass->tokenRef(ast::PointToVariableIdentifier, clsnameref);
            
            lexer->getNextToken();
            while (lexer->getCurrentToken() != '}') {
                report_eof();
                parseClassMember(newclass);
            }
            
            lexer->getNextToken();
        } else {
            report_eof();
            diags.reportError(diag::ExpectedOpenBrace, &clsbodyref);
            abort_parse();
        }
        
    } else {
        report_eof();
        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &clsnameref);
        abort_parse();
    }
    return true;
}

bool parser::ParserInstance::parseProtocol(ast::NameSpaceDecl *current) {
    src::SrcLoc prtnameref;
    if (lexer->getNextToken(&prtnameref) == lexer::TokenIdentifier) {
        std::string protocolname = lexer->getCurrentIdentifier();
        std::string baseprotocol = "";
        
        lexer->getNextToken();
        lexer->ignoreRelativePronounsIfAny();
        if (lexer->getCurrentToken() == lexer::TokenExtends || lexer->getCurrentToken() == ':') {
            src::SrcLoc superprtref;
            if (lexer->getNextToken(&superprtref) == lexer::TokenIdentifier) {
                baseprotocol = lexer->getCurrentIdentifier();
                lexer->getNextToken();
            } else {
                report_eof();
                diags.reportError(diag::ExpectedUnqualifiedIdentifier, &superprtref);
                abort_parse();
            }
        }
        
        src::SrcLoc prtbodyref;
        if (lexer->getCurrentToken(&prtbodyref) == '{') {
            
            while (lexer->getNextToken() != '}') {
                report_eof();
                // methods parsing
            }
            
            current->addProtocol(new ast::ProtocolDecl(protocolname, baseprotocol));
            
            lexer->getNextToken();
        } else {
            report_eof();
            diags.reportError(diag::ExpectedOpenBrace, &prtbodyref);
            abort_parse();
        }
        
    } else {
        report_eof();
        diags.reportError(diag::ExpectedUnqualifiedIdentifier, &prtnameref);
        abort_parse();
    }
    return true;
}

static std::string identifierByNamechain(std::vector<std::string> &namechain) { //FIXME: Stated function notation is a BETA feature
    if (!namechain.size()) return "";
    
    std::string idn = namechain[0];
    for (int i = 1; i < namechain.size(); i++) idn += "_"+ namechain[i];
    return idn;
}

bool parser::ParserInstance::parseStatedFunction(ast::NameSpaceDecl *current) { // TODO token ref emission
    std::vector<std::string> namechain;
    
    std::vector<ast::ParamVar *> args;
    ast::Type *returnType = nullptr;
    
    while (lexer->getCurrentToken() != '{') {
        report_eof();
        switch (lexer->getCurrentToken()) {
            case lexer::TokenIdentifier:
                namechain.push_back(lexer->getCurrentIdentifier());
                lexer->getNextToken();
                break;
            case '(': {
                if (lexer->getNextToken() != lexer::TokenFunction) {
                    lexer->ignoreArticleIfAny();
                    
                    ast::Type *argtype = parseType();
                    report_eof();
                    
                    if (lexer->getCurrentToken() == lexer::TokenIdentifier) {
                        std::string argname = lexer->getCurrentIdentifier();
                        
                        args.push_back(new ast::ParamVar(argname, argtype, nullptr));
                        
                        if (lexer->getNextToken() != ')') {
                            report_eof();
                            diags.reportError(diag::ExpectedClosedTuple);
                            abort_parse();
                        } else lexer->getNextToken();
                    } else {
                        report_eof();
                        diags.reportError(diag::ExpectedUnqualifiedIdentifier);
                        abort_parse();
                    }
                } else {
                    lexer->getNextToken();
                    lexer->ignoreRelativePronounsIfAny();
                    
                    if (lexer->getCurrentToken() == lexer::TokenReturnQualifier) {
                        lexer->getNextToken();
                        lexer->ignoreArticleIfAny();
                        returnType = parseType();
                        report_eof();
                    }
                    
                    if (lexer->getCurrentToken() != ')') {
                        report_eof();
                        diags.reportError(diag::ExpectedClosedTuple);
                        abort_parse();
                    } else lexer->getNextToken();
                }
                break;
            }
        }
    }
    
    if (!returnType) returnType = ast::BuiltinType::get(ast::BuiltinType::Void);
    
    ast::FunctionDecl *nfunc = builder.createFunctionDecl(identifierByNamechain(namechain), args, returnType);
    boundwrapper->addDeclaration(nfunc);
    
    ast::CompoundStmt *fblock;
    if (!parseCompoundStatement(fblock, nfunc)) return false; // FIXME declarations without definitions
    
    nfunc->setStatementsBlock(fblock);
    return true;
}

bool parser::ParserInstance::parseTopLevel(ast::NameSpaceDecl *current) {
    if (!current) current = getBoundAST()->getRootNameSpace();
    builder.setInsertNameSpace(current);
    
    if (lexer::isDelimiter(lexer->getCurrentToken())) {
        lexer->getNextToken();
    }
    
    switch (lexer->getCurrentToken()) {
        case lexer::TokenNameSpace:
            return parseNameSpace(current);
        case lexer::TokenLet:
            return parseGlobalLetDecl(current);
        case lexer::TokenAlias:
            return parseAliasDecl(current);
        case lexer::TokenFunction:
        case lexer::TokenNostalgic:
            return parseFunction(current);
        case lexer::TokenClass:
            return parseClass(current);
        case lexer::TokenProtocol:
            return parseProtocol(current);
        case lexer::TokenIdentifier:
            return parseStatedFunction(current);
    }
    return true;
}

