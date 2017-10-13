
#ifndef __ast_node
#error "__ast_node not defined before including typenodes.inc"
#endif

#ifndef __ast_begin_subclass
#error "__ast_begin_subclass not defined before including typenodes.inc"
#endif

#ifndef __ast_end_subclass
#error "__ast_end_subclass not defined before including typenodes.inc"
#endif

//__ast_node(CLASS, PARENT)
//__ast_begin_subclass(CLASS)
//__ast_end_subclass(CLASS)


__ast_node(Type, Component)
__ast_begin_subclass(Type)
    __ast_node(BuiltinType, Type)
    __ast_node(TypeEncloser, Type)
    __ast_begin_subclass(TypeEncloser)
        __ast_node(TypeRef, TypeEncloser)
        __ast_node(QualifiedType, TypeEncloser)
        __ast_node(AliasedType, TypeEncloser)
        __ast_end_subclass(TypeEncloser)
    __ast_node(PointerType, Type)
    __ast_node(ClassType, Type)
//    __ast_node(FunctionType, Type)
    __ast_end_subclass(Type)
