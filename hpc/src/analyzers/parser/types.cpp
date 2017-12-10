// => src/analyzers/parser/types.cpp
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
#include <hpc/ast/types/builtintype.h>

#define break_while() { shouldExit = true; continue; }

using namespace hpc;


ast::TypeQualifiers *parser::ParserInstance::parseQualifiers(bool report) {
    source::SrcLoc lastidref;
    
    ast::TypeQualifiers *typeQuals = new ast::TypeQualifiers();
    
    while (1) {
        switch (lexer->getCurrentToken(&lastidref)) {
            case lexer::TokenConstant: {
                if (typeQuals->isConstant()) {
                    //diags.reportWarning(diag::DuplicateConstantAttribute, &lastidref);
                } else typeQuals->setConstant(true);
                break;
            }

            default:
                return typeQuals;
        }
        
        lexer->getNextToken();
    }
    
    return typeQuals;
}

ast::Type *parser::ParserInstance::parseType(bool report) {
    ast::Type *parsedType = nullptr;
    
    source::SrcLoc lastidref;

    ast::TypeQualifiers *typeQuals = parseQualifiers();
    if (!typeQuals) {
        return nullptr;
    }
    
    enum {
        QualDefault = 0,
        QualUnsigned,
        QualSigned
    } signQual = QualDefault;
    
    bool shouldExit = false;
    do {
        
        switch (lexer->getCurrentToken(&lastidref)) {
            case lexer::TokenUnsigned:
                if (signQual != QualDefault) {
                    if (signQual == QualUnsigned) {
                        //diags.reportWarning(diag::DuplicateUnsignedQualifier, &lastidref);
                    } else {
                        diags.reportError(diag::ConflictingUnsignedQualifier, &lastidref);
                        return nullptr;
                    }
                }
                signQual = QualUnsigned;
                lexer->getNextToken();
                break;
            case lexer::TokenSigned:
                if (signQual != QualDefault) {
                    if (signQual == QualSigned) {
                        //diags.reportWarning(diag::DuplicateSignedQualifier, &lastidref);
                    } else {
                        diags.reportError(diag::ConflictingSignedQualifier, &lastidref);
                        return nullptr;
                    }
                }
                signQual = QualSigned;
                lexer->getNextToken();
                break;
            case lexer::TokenPointer:
                if (!parsedType) {
                    diags.reportError(diag::ExpectedType, &lastidref);
                    return nullptr;
                } else {
                    // FIXME wrap with QualifiedType if needed.
                    parsedType = parsedType->pointerTo();
                }
                lexer->getNextToken();
                break;
                
            default:
                if (!parsedType) {
                    switch (lexer->getCurrentToken()) {
                        case lexer::TokenTypeVoid:
                            parsedType = ast::BuiltinType::get(ast::BuiltinType::Void);
                            break;
                        case lexer::TokenTypeBool:
                            parsedType = ast::BuiltinType::get(ast::BuiltinType::Boolean);
                            break;
                        case lexer::TokenTypeChar:
                            parsedType = ast::BuiltinType::get(ast::BuiltinType::Character);
                            break;
                        case lexer::TokenTypeByte:
                            parsedType = ast::BuiltinType::get(signQual == QualSigned ? ast::BuiltinType::SignedByte : ast::BuiltinType::UnsignedByte);
                            break;
                        case lexer::TokenTypeShort:
                            parsedType = ast::BuiltinType::get(signQual == QualUnsigned ? ast::BuiltinType::UnsignedShort : ast::BuiltinType::SignedShort);
                            break;
                        case lexer::TokenTypeInteger:
                            parsedType = ast::BuiltinType::get(signQual == QualUnsigned ? ast::BuiltinType::UnsignedInteger : ast::BuiltinType::SignedInteger);
                            break;
                        case lexer::TokenTypeLong:
                            // FIXME long long check
                            parsedType = ast::BuiltinType::get(signQual == QualUnsigned ? ast::BuiltinType::UnsignedLong : ast::BuiltinType::SignedLong);
                            break;
                        case lexer::TokenTypeFloat:
                            parsedType = ast::BuiltinType::get(ast::BuiltinType::Float);
                            break;
                        case lexer::TokenTypeDouble:
                            parsedType = ast::BuiltinType::get(ast::BuiltinType::Double);
                            break;
                        case lexer::TokenIdentifier:
                            parsedType = new ast::TypeRef(parseSymbol());
                            continue;
                    }
                    
                    if (!parsedType) {
                        if (lexer->eof()) return nullptr;
                        else break_while();
                    }
                    
                    if (signQual != QualDefault && !parsedType->isIntegerType()) {
                        diags.reportError(diag::TypeCannotBeSignedOrUnsigned, &lastidref) << parsedType->asString();
                    }
                    
                    if (parsedType && !typeQuals->isDefault()) {
                        ast::QualifiedType *qualType = new ast::QualifiedType(parsedType);
                        qualType->setConstant(typeQuals->isConstant());
                        parsedType = qualType;
                    }
                    
                } else if (lexer->eof()) {
                    return nullptr;
                } else break_while();
                
                lexer->getNextToken();
                break;
        }
        
    } while (!shouldExit);
    
    
    if (!parsedType) {
        
        source::SrcLoc curtkref;
        if (lexer->getCurrentToken(&curtkref) == lexer::TokenNull) {
            parsedType = ast::BuiltinType::get(ast::BuiltinType::Void);
            lexer->getNextToken();
        } else {
            diags.reportError(diag::ExpectedType, &curtkref);
            return nullptr;
        }
    }
    
    return parsedType;
}

