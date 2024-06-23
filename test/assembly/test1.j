.class public test1
.super java/lang/Object
.field public static a I
.field public static b I
.field public static c I
.field public static u I
.field public static d [I
.field public static e [F
.field public static g F
.field public static h F
.field public static x F
.field public static k [[F
.field public static l [[I
.method public static sort(I)V
.limit locals 50
.limit stack 50
    getstatic test1/b I
    putstatic test1/c I
Lf:
    return
.end method
.method public static addition(II)I
.limit locals 50
.limit stack 50
    iload_1
    iload_0
    iadd
Lf:
    ireturn
.end method
.method public static ggyy(FF)F
.limit locals 50
.limit stack 50
    iload_1
    iload_0
    iadd
Lf:
    freturn
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic test1/a I
    ldc 0
    putstatic test1/b I
    ldc 0
    putstatic test1/c I
    ldc 0
    putstatic test1/u I
    ldc 10
    multianewarray [I 1
    putstatic test1/d [I
    ldc 10
    multianewarray [F 1
    putstatic test1/e [F
    ldc 0.0
    putstatic test1/g F
    ldc 0.0
    putstatic test1/h F
    ldc 0.0
    putstatic test1/x F
    ldc 57
    ldc 57
    multianewarray [[F 2
    putstatic test1/k [[F
    ldc 57
    ldc 57
    multianewarray [[I 2
    putstatic test1/l [[I
    ldc 8
    invokestatic test1/addition(II)I
    putstatic test1/c I
    getstatic test1/c I
    invokestatic io/writelnI(I)V
    getstatic test1/c I
    invokestatic io/writelnI(I)V
    invokestatic test1/ggyy(FF)F
    putstatic test1/x F
    getstatic test1/x F
    invokestatic io/writelnR(F)V
    ldc 0
    putstatic test1/b I
    getstatic test1/b I
    getstatic test1/a I
    if_icmpgt L1
    goto L2
L1:
L1:
    getstatic test1/a I
    invokestatic io/writelnI(I)V
    getstatic test1/b I
    getstatic test1/a I
    if_icmpeq L2
    goto L3
L2:
L2:
    getstatic test1/a I
    invokestatic io/writelnI(I)V
    getstatic test1/b I
    getstatic test1/a I
    if_icmpeq L3
    goto L4
L3:
L3:
    getstatic test1/a I
    invokestatic io/writelnI(I)V
    getstatic test1/b I
    getstatic test1/a I
    if_icmpge L4
    goto L5
L4:
L4:
    getstatic test1/a I
    invokestatic io/writelnI(I)V
    getstatic test1/b I
    getstatic test1/a I
    if_icmple L5
    goto L6
L5:
L5:
    getstatic test1/a I
    invokestatic io/writelnI(I)V
    getstatic test1/b I
    ldc 0
    if_icmpne L6
    goto L7
L6:
L6:
    getstatic test1/a I
    invokestatic io/writelnI(I)V
    ldc 1
    putstatic test1/a I
L7:
    getstatic test1/a I
    ldc 5
    if_icmpeq L8
    goto L9
L8:
    getstatic test1/a I
    invokestatic io/writelnI(I)V
    ldc 1
    getstatic test1/a I
    iadd
    putstatic test1/a I
    goto L7
L9:
    getstatic test1/e [F
    ldc 3
    ldc 1
    isub
    ldc 9
    iastore
    getstatic test1/k [[F
    ldc 25
    ldc 1
    isub
    aaload
    ldc 26
    ldc 1
    isub
    ldc 3
    iastore
    getstatic test1/l [[I
    ldc 28
    ldc 1
    isub
    aaload
    ldc 26
    ldc 1
    isub
    ldc 88
    iastore
    getstatic test1/a I
    invokestatic io/writelnI(I)V
    getstatic test1/d    putstatic test1/u I
    getstatic test1/u I
    invokestatic io/writelnI(I)V
    getstatic test1/e    putstatic test1/g F
    getstatic test1/g F
    invokestatic io/writelnR(F)V
    getstatic test1/k    putstatic test1/g F
    getstatic test1/g F
    invokestatic io/writelnR(F)V
    getstatic test1/l    putstatic test1/u I
    getstatic test1/u I
    invokestatic io/writelnI(I)V
    ldc 9
    ldc 9
    iadd
    ldc 10
    isub
    putstatic test1/u I
    getstatic test1/u I
    invokestatic io/writelnI(I)V
    ldc 9
    ldc 9
    imul
    ldc 2
    iadd
    ldc 5
    idiv
    ldc 3
    iadd
    ldc 1
    isub
    getstatic test1/u I
    iadd
    putstatic test1/u I
    getstatic test1/u I
    invokestatic io/writelnI(I)V
    return
.end method
