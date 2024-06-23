.class public test_nestedStmt
.super java/lang/Object
.field public static a I
.field public static b I
.field public static d I
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic test_nestedStmt/a I
    ldc 0
    putstatic test_nestedStmt/b I
    ldc 0
    putstatic test_nestedStmt/d I
    ldc 1
    putstatic test_nestedStmt/a I
    ldc 7
    putstatic test_nestedStmt/b I
    ldc 9
    putstatic test_nestedStmt/d I
    getstatic test_nestedStmt/a I
    getstatic test_nestedStmt/b I
    ldc 3
    if_icmpeq L1
    goto L2
    iaddL2
    goto L3
L2:
    getstatic test_nestedStmt/d I
    ldc 1000
    iadd
    goto Lf
    getstatic test_nestedStmt/a I
    invokestatic io/writelnI(I)V
    getstatic test_nestedStmt/b I
    invokestatic io/writelnI(I)V
    getstatic test_nestedStmt/d I
    invokestatic io/writelnI(I)V
    return
.end method
