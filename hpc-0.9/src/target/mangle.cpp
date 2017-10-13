//
//  mangle.cpp
//  HumanPlus
//
//  Created by Lore on 14/05/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#include <hpc/target/mangle.h>
#include <hpc/ast/ast.h>

#include <sstream>

using namespace hpc;



std::string target::NeuronMangle::mangleNameSpace(ast::NameSpaceDecl *nameSpace) {
    if (!nameSpace || !nameSpace->getContainer()) return "";
    
    std::ostringstream mangledName;
    mangledName << mangleNameSpace(nameSpace->getContainer()) << nameSpace->getName().length() << nameSpace->getName();
    return mangledName.str();
}

std::string target::NeuronMangle::mangleFunction(ast::FunctionDecl *function) {
    if (function->isMainFunction()) return "main";
    if (function->isNostalgic()) return function->getName(); // FIXME the mangle spec shouldn't even be used.
    
    std::ostringstream mangledName;
    mangledName << "__HF";
    std::string nsMangle = mangleNameSpace(function->getContainer());
    if (!nsMangle.empty())
        mangledName << nsMangle;
    
    mangledName << function->getName().length() << function->getName() << "T";
    
    if (!function->getArgs().size()) mangledName << "v";
    else for (ast::ParamVar *arg : function->getArgs())
        mangledName << mangleType(arg->getType());
    
    return mangledName.str();
}

std::string target::NeuronMangle::mangleGlobalVariable(ast::GlobalVar *variable) {
    std::ostringstream mangledName;
    mangledName << "__HG";
    std::string nsMangle = mangleNameSpace(variable->getContainer());
    if (!nsMangle.empty())
        mangledName << nsMangle;
    
    mangledName << variable->getName().length() << variable->getName();
    
    return mangledName.str();
}

std::string target::NeuronMangle::mangleType(ast::Type *type) {
    if (!type->isCanonicalType()) {
        return mangleType(type->getCanonicalType());
    }
    
    if (ast::BuiltinType *builtinType = llvm::dyn_cast<ast::BuiltinType>(type)) {
        switch (builtinType->getBuiltinTypeID()) {
            case ast::BuiltinType::Void:                return "v";
            case ast::BuiltinType::Boolean:             return "b";
            case ast::BuiltinType::Character:           return "cb";
            case ast::BuiltinType::SignedByte:          return "sb";
            case ast::BuiltinType::UnsignedByte:        return "ub";
            case ast::BuiltinType::UTF16Character:      return "cs";
            case ast::BuiltinType::SignedShort:         return "ss";
            case ast::BuiltinType::UnsignedShort:       return "us";
            case ast::BuiltinType::UTF32Character:      return "ci";
            case ast::BuiltinType::SignedInteger:       return "si";
            case ast::BuiltinType::UnsignedInteger:     return "ui";
            case ast::BuiltinType::SignedLong:          return "sl";
            case ast::BuiltinType::UnsignedLong:        return "ul";
            case ast::BuiltinType::SignedLongLong:      return "sx";
            case ast::BuiltinType::UnsignedLongLong:    return "ux";
            case ast::BuiltinType::Float:               return "f";
            case ast::BuiltinType::Double:              return "d";
            case ast::BuiltinType::String:              return "r";
        }
        return "v";
    }
    
    if (ast::PointerType *pointerType = llvm::dyn_cast<ast::PointerType>(type)) {
        // FIXME pointer quals
        return "P" + mangleType(pointerType->getPointedType());
    }
    
    //if (type->isConstant()) mng += "K";
    
    return "";// FIXME
}

std::string target::NeuronMangle::mangleClass(ast::ClassDecl *classDecl) {
    std::ostringstream os;
    os << "class.H" << mangleNameSpace(classDecl->getContainer()) << classDecl->getName().length() << classDecl->getName();
    
    return os.str();
}
