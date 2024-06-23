.class public test_function
.super java/lang/Object
.field public static a I
.field public static b I
.field public static c F
.method public static ss(I)I
.limit locals 50
.limit stack 50
    iload 0
Lf:
    ireturn
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic test_function/a I
    ldc 0
    putstatic test_function/b I
    ldc 0.0
    putstatic test_function/c F
    ldc 3
    invokestatic test_function/ss(I)I
    invokestatic test_function/ss(I)I
    invokestatic test_function/ss(I)I
    invokestatic test_function/ss(I)I
    putstatic test_function/a I
    getstatic test_function/a I
    invokestatic io/writelnI(I)V
    return
.end method
