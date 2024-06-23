.class public test2
.super java/lang/Object
.field public static aa I
.field public static bb I
.field public static cc I
.field public static g [[[I
.field public static r [[I
.method public static ss()V
.limit locals 50
.limit stack 50
Lf:
    return
.end method
.method public static tt(IIFF)I
.limit locals 50
.limit stack 50
    ldc 5
    ldc 4
    imul
    ldc 3
    iadd
    putstatic test2/z I
    getstatic test2/z I
    invokestatic io/writelnI(I)V
    ldc 5
    iload_0
    iadd
Lf:
    ireturn
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic test2/aa I
    ldc 0
    putstatic test2/bb I
    ldc 0
    putstatic test2/cc I
    ldc 1
    ldc 3
    ldc 5
    multianewarray [[[I 3
    putstatic test2/g [[[I
    ldc 1
    ldc 5
    multianewarray [[I 2
    putstatic test2/r [[I
    getstatic test2/bb I
    ldc 3
    idiv
    ldc 2
    imul
    ldc 4
    iadd
    getstatic test2/aa I
    isub
    putstatic test2/aa I
    getstatic test2/aa I
    invokestatic io/writelnI(I)V
    getstatic test2/bb I
    invokestatic io/writelnI(I)V
    getstatic test2/aa I
    getstatic test2/bb I
    if_icmple L1
    goto L2
L2:
    getstatic test2/aa I
    ldc 0
    if_icmpeq L3
    goto L4
L3:
L4:
L5:
    getstatic test2/aa I
    invokestatic io/writelnI(I)V
L6:
    getstatic test2/bb I
    ldc 5
    if_icmpeq L7
    goto L8
L7:
    ldc 0
    putstatic test2/aa I
L8:
    getstatic test2/aa I
    ldc 5
    if_icmpeq L9
    goto L10
L9:
    ldc 1
    getstatic test2/aa I
    iadd
    putstatic test2/aa I
    goto L8
L10:
    ldc 1
    getstatic test2/bb I
    iadd
    putstatic test2/bb I
    getstatic test2/bb I
    invokestatic io/writelnI(I)V
    goto L6
L10:
    getstatic test2/g [[[I
    ldc 0
    ldc 1
    isub
    aaload
    ldc 3
    ldc 1
    isub
    aaload
    ldc 4
    ldc 1
    isub
    iaload
    invokestatic io/writelnI(I)V
    return
.end method
