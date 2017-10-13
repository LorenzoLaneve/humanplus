//
//  EnumDecl.h
//  Human Plus
//
//  Created by Lore on 12/08/15.
//  Copyright (c) 2015 Snow Apps inc. All rights reserved.
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
