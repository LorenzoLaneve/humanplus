
#ifndef __ast_node
#error "__ast_node not defined before including declnodes.inc"
#endif

#ifndef __ast_begin_subclass
#error "__ast_begin_subclass not defined before including declnodes.inc"
#endif

#ifndef __ast_end_subclass
#error "__ast_end_subclass not defined before including declnodes.inc"
#endif

//__ast_node(CLASS, PARENT)
//__ast_begin_subclass(CLASS)
//__ast_end_subclass(CLASS)


__ast_node(Decl, Component)
__ast_begin_subclass(Decl)
    __ast_node(NamedDecl, Decl)
    __ast_begin_subclass(NamedDecl)
        __ast_node(GlobalDecl, Decl)
        __ast_begin_subclass(GlobalDecl)
            __ast_node(NameSpaceDecl, Decl)
            __ast_node(Var, Decl)
            __ast_begin_subclass(Var)
                __ast_node(GlobalVar, Var)
                __ast_begin_subclass(GlobalVar)
                    __ast_node(FieldDecl, GlobalVar)
                    __ast_end_subclass(GlobalVar)
                __ast_node(LocalVar, Var)
                __ast_node(ParamVar, Var)
                __ast_end_subclass(Var)
            __ast_node(FunctionDecl, GlobalDecl)
            __ast_node(TypeAliasDecl, GlobalDecl)
            __ast_node(ClassDecl, GlobalDecl)
            __ast_node(ProtocolDecl, GlobalDecl)
            __ast_end_subclass(GlobalDecl)
        __ast_end_subclass(NamedDecl)
    __ast_end_subclass(Decl)
