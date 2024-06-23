.class public test_recursion
.super java/lang/Object
.field public static a I
.field public static b I
.field public static c F
.method public static fact(I)I
.limit locals 50
.limit stack 50
    iload_0
    ldc 1
    if_icmpeq L1
    goto L2
L1:
    iload_0
    goto Lf
L2:
    iload_0
    iload_0
    ldc 1
    isub
    invokestatic test_recursion/fact(I)I
    imul
    goto Lf
Lf:
    ireturn
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic test_recursion/a I
    ldc 0
    putstatic test_recursion/b I
    ldc 0.0
    putstatic test_recursion/c F
    ldc 4
    invokestatic test_recursion/fact(I)I
    putstatic test_recursion/a I
    getstatic test_recursion/a I
    invokestatic io/writelnI(I)V
    return
.end method
