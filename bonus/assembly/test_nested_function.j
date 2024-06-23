.class public test_nested_function
.super java/lang/Object
.field public static a I
.field public static b I
.method public static ss(I)I
.limit locals 50
.limit stack 50
    ldc 4
    iload_0
    isub
    putstatic test_nested_function/d I
Lf:
    ireturn
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic test_nested_function/a I
    ldc 0
    putstatic test_nested_function/b I
    ldc 5
    putstatic test_nested_function/b I
    ldc 3
    invokestatic test_nested_function/ss(I)I
    putstatic test_nested_function/a I
    getstatic test_nested_function/a I
    invokestatic io/writelnI(I)V
    return
.end method
