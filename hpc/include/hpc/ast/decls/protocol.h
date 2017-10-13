// => hpc/ast/decls/protocol.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
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
