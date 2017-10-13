//
//  ProtocolDecl.h
//  Human Plus
//
//  Created by Lore on 11/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
//

#ifndef __human_plus_compiler_ast_protocol
#define __human_plus_compiler_ast_protocol

#include <hpc/ast/decls/namespace.h>

#include <string>

namespace hpc {
    namespace ast {
        
        class ProtocolDecl : public NameSpaceDecl { // FIXME ProtocolDecl should inherit NameSpaceDecl
            
            std::string base; // FIXME Symbols should be used.
            
        public:
            ProtocolDecl(std::string name, std::string base) : NameSpaceDecl(name), base(base) {  }
            virtual ~ProtocolDecl() {  }
            

            llvm_rtti_impl(ProtocolDecl);
        };
        
    }
}

#endif
