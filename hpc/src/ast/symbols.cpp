// => src/ast/symbols.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ast/symbols.h>

#include <sstream>

using namespace hpc;

ast::Symbol::Symbol(std::string symroot, src::SrcLoc *srcloc) {
    pushBackChild(symroot, srcloc);
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

void ast::Symbol::pushBackChild(std::string childsym, src::SrcLoc *srcloc) {
    sympath.push_back({childsym, srcloc ? new src::SrcLoc(*srcloc) : nullptr });
}

void ast::Symbol::pushFrontChild(std::string childsym, src::SrcLoc *srcloc) {
    sympath.insert(sympath.begin(), {childsym, srcloc ? new src::SrcLoc(*srcloc) : nullptr });
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
