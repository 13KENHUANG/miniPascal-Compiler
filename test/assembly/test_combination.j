.class public test_combination
.super java/lang/Object
.field public static a I
.field public static b I
.method public static comb(II)I
.limit locals 50
.limit stack 50
    iload_0
    iload_1
    if_icmpgt L1
    goto L2
L2:
    iload_0
    ldc 1
    if_icmpeq L3
    goto L4
L3:
    ldc 1
    goto Lf
L4:
    ldc 1
    goto Lf
Lf:
    ireturn
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic test_combination/a I
    ldc 0
    putstatic test_combination/b I
    ldc 3
    putstatic test_combination/a I
    ldc 4
    putstatic test_combination/b I
    invokestatic test_combination/comb(II)I
    invokestatic io/writelnI(I)V
    return
.end method
