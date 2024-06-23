.class public test_global1
.super java/lang/Object
.field public static a I
.field public static b I
.field public static c F
.method public static ss()I
.limit locals 50
.limit stack 50
    getstatic test_global1/b I
    getstatic test_global1/a I
    iadd
Lf:
    ireturn
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic test_global1/a I
    ldc 0
    putstatic test_global1/b I
    ldc 0.0
    putstatic test_global1/c F
    ldc 3
    putstatic test_global1/a I
    ldc 4
    putstatic test_global1/b I
    invokestatic test_global1/ss()I
    putstatic test_global1/b I
    getstatic test_global1/b I
    invokestatic io/writelnI(I)V
    return
.end method
