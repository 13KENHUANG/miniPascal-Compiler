.class public test_global3
.super java/lang/Object
.field public static a I
.field public static b I
.field public static c F
.method public static tt()I
.limit locals 50
.limit stack 50
    getstatic test_global3/a [I
    ldc 1
    ldc 1
    isub
    ldc 10
    iastore
    getstatic test_global3/b [I
    ldc 1
    ldc 1
    isub
    ldc 11
    getstatic test_global3/a [I
    ldc 1
    ldc 1
    isub
    iaload
    iadd
    iastore
    getstatic test_global2/b I
Lf:
    ireturn
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic test_global3/a I
    ldc 0
    putstatic test_global3/b I
    ldc 0.0
    putstatic test_global3/c F
    ldc 3
    putstatic test_global3/a I
    ldc 4
    putstatic test_global3/b I
    invokestatic test_global3/tt()I
    invokestatic io/writelnI(I)V
    return
.end method
