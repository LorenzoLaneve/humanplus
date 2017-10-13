//
//  symbols.cpp
//  HumanPlus
//
//  Created by Lore on 03/04/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/ast/symbols.h>

#include <sstream>

using namespace hpc;

ast::Symbol::Symbol(std::string symroot, source::TokenRef *tkref) {
    pushBackChild(symroot, tkref);
}

ast::Symbol::Symbol(ast::SymbolIdentifier &symbolID) : Symbol(symbolID.identifier, symbolID.symref) {  }

ast::Symbol ast::Symbol::containedSymbol() const {
    Symbol symclone = *this;
    symclone.sympath.erase(symclone.sympath.begin());
    return symclone;
}

ast::Symbol ast::Symbol::containerSymbol() const {
    Symbol symclone = *this;
    symclone.sympath.pop_back();
    return symclone;
}

void ast::Symbol::pushBackChild(std::string childsym, source::TokenRef *tkref) {
    sympath.push_back({childsym, tkref ? new source::TokenRef(*tkref) : nullptr });
}

void ast::Symbol::pushFrontChild(std::string childsym, source::TokenRef *tkref) {
    sympath.insert(sympath.begin(), {childsym, tkref ? new source::TokenRef(*tkref) : nullptr });
}

std::string ast::Symbol::str() const {
    std::ostringstream ostr;
    int i = 0;
    for (const SymbolIdentifier &symcomponent : sympath) {
        if (i++) ostr << "::";
        ostr << symcomponent.identifier;
    }
    
    return ostr.str();
}
