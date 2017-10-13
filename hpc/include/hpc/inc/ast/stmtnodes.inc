
#ifndef __ast_node
#error "__ast_node not defined before including stmtnodes.inc"
#endif

#ifndef __ast_begin_subclass
#error "__ast_begin_subclass not defined before including stmtnodes.inc"
#endif

#ifndef __ast_end_subclass
#error "__ast_end_subclass not defined before including stmtnodes.inc"
#endif

//__ast_node(CLASS, PARENT)
//__ast_begin_subclass(CLASS)
//__ast_end_subclass(CLASS)


__ast_node(Stmt, Component)
__ast_begin_subclass(Stmt)
    __ast_node(CompoundStmt, Stmt)
    __ast_node(VarDeclStmt, Stmt)
    __ast_node(ReturnStmt, Stmt)
    __ast_node(BreakStmt, Stmt)
    __ast_node(ContinueStmt, Stmt)
    __ast_node(IfStmt, Stmt)
    __ast_node(SimpleIterStmt, Stmt)
    __ast_begin_subclass(SimpleIterStmt)
        __ast_node(PreWhileStmt, SimpleIterStmt)
        __ast_node(PostWhileStmt, SimpleIterStmt)
        __ast_node(PreUntilStmt, SimpleIterStmt)
        __ast_node(PostUntilStmt, SimpleIterStmt)
        __ast_node(ForStmt, SimpleIterStmt)
        __ast_end_subclass(SimpleIterStmt)
    __ast_node(Expr, Stmt)
    __ast_begin_subclass(Expr)
        __ast_node(ImplicitCastExpr, Expr)
        __ast_node(EvalExpr, Expr)
        __ast_node(BinaryExpr, Expr)
        __ast_begin_subclass(BinaryExpr)
            __ast_node(ArithmeticExpr, BinaryExpr)
            __ast_node(ComparisonExpr, BinaryExpr)
            __ast_node(AssignmentExpr, BinaryExpr)
            __ast_node(BitwiseExpr, BinaryExpr)
            __ast_end_subclass(BinaryExpr)
        __ast_node(UnaryExpr, Expr)
        __ast_begin_subclass(UnaryExpr)
            __ast_node(ArithmeticNegationExpr, UnaryExpr)
            __ast_node(LogicalNegationExpr, UnaryExpr)
            __ast_node(BitwiseNegationExpr, UnaryExpr)
            __ast_end_subclass(UnaryExpr)
        __ast_node(VarRef, Expr)
        __ast_node(FunctionCall, Expr)
        __ast_node(FieldRef, Expr)
        __ast_node(Constant, Expr)
        __ast_begin_subclass(Constant)
            __ast_node(CharLiteral, Constant)
            __ast_node(IntegerLiteral, Constant)
            __ast_node(UIntegerLiteral, Constant)
            __ast_node(LongLiteral, Constant)
            __ast_node(ULongLiteral, Constant)
            __ast_node(FloatLiteral, Constant)
            __ast_node(DoubleLiteral, Constant)
            __ast_node(BoolLiteral, Constant)
            __ast_node(StringLiteral, Constant)
            __ast_node(NullPointer, Constant)
            __ast_end_subclass(Constant)
        __ast_end_subclass(Expr)
    __ast_end_subclass(Stmt)
