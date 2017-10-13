// => hpc/ast/decls/enumeration.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_enum
#define __human_plus_compiler_ast_enum

#include <hpc/ast/decls/declaration.h>
#include <hpc/ast/decls/variable.h>
#include <hpc/ast/unit.h>

#include <string>
#include <vector>

namespace hpc {
    namespace ast {
        
        class EnumDecl : public GlobalDecl {

        public:
            EnumDecl(std::string name) : GlobalDecl(name) {  }
            
            virtual ~EnumDecl() {  }

            
            //llvm_rtti_impl(EnumDecl);
        };
        
    }
}

#endif
