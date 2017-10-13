//
//  parser.cpp
//  Human Plus
//
//  Created by Lore on 08/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/analyzers/parser/parser.h>
#include <hpc/diagnostics/diagnostics.h>

using namespace hpc;

parser::ParserInstance::ParserInstance(diag::DiagEngine &diags, ast::AbstractSyntaxTree *boundAST) : diags(diags), boundAST(boundAST), builder(boundAST) {
    assert(boundAST && "No syntax tree object was passed to the parser.");
}

void parser::ParserInstance::bindAST(ast::AbstractSyntaxTree *astobj) {
    boundAST = astobj;
}

bool parser::ParserInstance::bindSourceFile(source::SourceFile *inputFile) {
    if (!inputFile->isOk()) {
        diags.reportError(diag::ErrorOpeningFile) << inputFile->getFileName();
        
        builder.setInsertUnit(nullptr);
        return false;
    }
    
    if (!inputFile->exists()) {
        diags.reportError(diag::NoSuchFileOrDirectory) << inputFile->getFileName();
        
        builder.setInsertUnit(nullptr);
        return false;
    }
    
    lexer = new lexer::LexerInstance(diags, inputFile);
    builder.setInsertUnit(builder.getOrCreateUnit(inputFile));
    
    boundwrapper = lexer->getSourceFile()->getModuleWrapper();
    
    return true;
}

source::SourceFile *parser::ParserInstance::getSourceFile() {
    assert(lexer && "No lexer.");
    assert(lexer->getSourceFile() && "No source file.");
    return lexer->getSourceFile();
}

void parser::ParserInstance::unbindSourceFile() {
    lexer->unbind();
    
    boundwrapper = nullptr;
}

modules::ModuleWrapper *parser::ParserInstance::getBoundModuleWrapper() {
    assert(boundwrapper && "No module wrapper bound to the parser.");
    return boundwrapper;
}

void parser::ParserInstance::parse() {
    diag::DiagnosticsReport fileReport;
    diags.addReport(fileReport);
    
    lexer->getNextToken();
    while (lexer->getCurrentToken() != lexer::TokenEOF) {
        if (!parseTopLevel(getBoundAST()->getRootNameSpace())) {
            diags.reportError(diag::UnexpectedEOF, lexer->getSourceFile()->getCaret());
        }
    }
    
    diags.printAndRemoveReport(fileReport);
}
