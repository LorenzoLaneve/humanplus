//
//  visitor.h
//  HumanPlus
//
//  Created by Lore on 02/02/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_ast_visitor
#define __human_plus_compiler_ast_visitor

#include <hpc/ast/ast.h>

#include <llvm/Support/Casting.h>

namespace hpc {
    
    namespace ast {

        class AbstractSyntaxTree;
        class CompilationUnit;
        class Component;
        
        /*
#define __ast_node(CLASS, PARENT) class CLASS;
#define __ast_begin_subclass(CLASS)
#define __ast_end_subclass(CLASS)
        
#include <hpc/inc/ast/typenodes.inc>
#include <hpc/inc/ast/declnodes.inc>
#include <hpc/inc/ast/stmtnodes.inc>
        
#undef __ast_node
#undef __ast_begin_subclass
#undef __ast_end_subclass
        */
        
#define TRY_TO(method) getSubClass().method;
        
        
        template <typename SubClass, typename RetType = void>
        class RecursiveVisitor {

        public:
            typedef RetType ReturnType;
            
            // Workaround for virtual templated methods
            SubClass &getSubClass() { return *static_cast<SubClass *>(this); }
            
            
            ReturnType visitComponent(Component *component) {  }

#define __ast_node(CLASS, PARENT) \
            ReturnType visit##CLASS(CLASS *component) { return TRY_TO(visit##PARENT(component)); }
#define __ast_begin_subclass(CLASS)
#define __ast_end_subclass(CLASS)
            
#include <hpc/inc/ast/declnodes.inc>
#include <hpc/inc/ast/typenodes.inc>
#include <hpc/inc/ast/stmtnodes.inc>
            
#undef __ast_node
            
            
            ReturnType take(Component *component) {
                if (llvm::isa<Decl>(component)) {
                    return takeDecl(static_cast<Decl*>(component));
                }
                
                if (llvm::isa<Type>(component)) {
                    return takeType(static_cast<Type*>(component));
                }
                
                if (llvm::isa<Stmt>(component)) {
                    return takeStmt(static_cast<Stmt*>(component));
                }
            }
            
        
#define __ast_node(CLASS, PARENT) case ASTCK_##CLASS: return TRY_TO(visit##CLASS(static_cast<CLASS *>(component))); break;
            
            ReturnType takeDecl(Decl *component) {
                switch (component->getKind()) {
                        
#include <hpc/inc/ast/declnodes.inc>
                        
                    default: llvm_unreachable("Type not recognized.");
                }
            }
        
            ReturnType takeType(Type *component) {
                switch (component->getKind()) {
                        
#include <hpc/inc/ast/typenodes.inc>
                        
                    default: llvm_unreachable("Type not recognized.");
                }
            }
            
            ReturnType takeStmt(Stmt *component) {
                switch (component->getKind()) {
                    
#include <hpc/inc/ast/stmtnodes.inc>
                        
                    default: llvm_unreachable("Type not recognized.");
                }
            }
            
#undef __ast_node
#undef __ast_begin_subclass
#undef __ast_end_subclass
            
        };
        
    }
}

#endif
