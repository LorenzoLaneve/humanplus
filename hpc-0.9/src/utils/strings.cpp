//
//  util.cpp
//  Human Plus
//
//  Created by Lore on 13/11/15.
//  Copyright © 2015 Snow Apps inc. All rights reserved.
//

#include <hpc/utils/strings.h>

#include <sstream>
#include <algorithm>

#include <cassert>

using namespace hpc;

std::vector<std::string> util::split(const std::string &s, char delim, bool truncempty) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) if (!truncempty || item.length()) elems.push_back(item);

    return elems;
}

std::string util::replace(std::string str, std::string from, std::string to) {
    std::string result = str;
    
    size_t stpos = 0;
    while ((stpos = result.find(from, stpos)) != std::string::npos) {
        result.replace(stpos, from.length(), to);
        stpos += to.length();
    }
    
    return result;
}

std::string util::extractClassName(std::string pf) {
    size_t i = pf.find("=");
    if (i != std::string::npos) {
        pf = pf.substr(0, i);
    }
    
    std::vector<std::string> v = split(pf, ':', true);
    
    return v[v.size() - 2];
}

bool util::startsWith(std::string str, std::string begin) {
    return str.substr(0, begin.length()) == begin;
}

bool util::isEmptyOrBlank(std::string str) {
    return std::all_of(str.begin(), str.end(), isspace);
}

std::string util::bashEncode(std::string str) {
    return replace(str, " ", "\\ ");
}

std::string util::replaceParams(std::string reason, std::vector<std::string> params) {
    assert(params.size() < 10 && "Too many parameters, diagnostics reporters can take up to 9 parameters.");
    
    std::ostringstream os;
    
    bool replacing = false;
    for (char &c : reason) {
        if (replacing) {
            unsigned int i = c - '0';
            
            if (i < params.size()) {
                os << params[i];
            } else {
                os << '%' << c;
            }
            replacing = false;
        } else {
            if (c == '%')
                replacing = true;
            else os << c;
        }
    }
    
    return os.str();
}
