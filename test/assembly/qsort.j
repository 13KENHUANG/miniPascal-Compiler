.class public qsort
.super java/lang/Object
.field public static i I
.field public static j I
.field public static tmp I
.field public static size I
.field public static arr [I
.method public static readArr()V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic qsort/size I
    invokestatic io/readlnI()I
    putstatic qsort/input I
L1:
    getstatic qsort/input I
    ldc 0
    if_icmpne L2
    goto L3
L2:
    ldc 1
    getstatic qsort/size I
    iadd
    putstatic qsort/size I
    getstatic qsort/arr [I
    getstatic qsort/input I
    iaload
    iastore
    invokestatic io/readlnI()I
    putstatic qsort/input I
    goto L1
L3:
Lf:
    return
.end method
.method public static split(II)I
.limit locals 50
.limit stack 50
    getstatic qsort/arr    putstatic qsort/pivot I
    ldc 1
    iload_0
    iadd
    putstatic qsort/left I
    getstatic qsort/stop I
    putstatic qsort/right I
L3:
    getstatic qsort/left I
    getstatic qsort/right I
    if_icmple L4
    goto L5
L4:
    ldc 1
    putstatic qsort/test I
    getstatic qsort/left I
    iload_1
    if_icmple L5
    goto L6
L5:
    getstatic qsort/test I
    ldc 1
    if_icmpeq L6
    goto L7
L7:
    getstatic qsort/pivot I
    if_icmpeq L8
    goto L9
L8:
L10:
    getstatic qsort/test I
    ldc 1
    if_icmpeq L11
    goto L12
L11:
    ldc 1
    getstatic qsort/left I
    iadd
    putstatic qsort/left I
    ldc 1
    putstatic qsort/test I
    getstatic qsort/left I
    iload_1
    if_icmple L12
    goto L13
L12:
    getstatic qsort/test I
    ldc 1
    if_icmpeq L13
    goto L14
L14:
    getstatic qsort/pivot I
    if_icmpeq L15
    goto L16
L15:
    goto L10
L17:
    ldc 1
    putstatic qsort/test I
    getstatic qsort/right I
    iload_0
    if_icmpgt L17
    goto L18
L17:
    getstatic qsort/test I
    ldc 1
    if_icmpeq L18
    goto L19
L19:
    getstatic qsort/pivot I
    if_icmpge L20
    goto L21
L20:
L22:
    getstatic qsort/test I
    ldc 1
    if_icmpeq L23
    goto L24
L23:
    ldc 1
    getstatic qsort/right I
    isub
    putstatic qsort/right I
    ldc 1
    putstatic qsort/test I
    getstatic qsort/right I
    iload_0
    if_icmpgt L24
    goto L25
L24:
    getstatic qsort/test I
    ldc 1
    if_icmpeq L25
    goto L26
L26:
    getstatic qsort/pivot I
    if_icmpge L27
    goto L28
L27:
    goto L22
L29:
    getstatic qsort/left I
    getstatic qsort/right I
    if_icmpeq L29
    goto L30
    goto L3
L30:
    getstatic qsort/arr    putstatic qsort/tmp I
    getstatic qsort/arr [I
    getstatic qsort/arr [I
    iaload
    iastore
    getstatic qsort/arr [I
    getstatic qsort/tmp I
    iaload
    iastore
    iload 0
Lf:
    ireturn
.end method
.method public static quicksortRecur(II)V
.limit locals 50
.limit stack 50
    iload_0
    iload_1
    if_icmpeq L30
    goto L31
Lf:
    return
.end method
.method public static quicksort()V
.limit locals 50
.limit stack 50
    invokestatic qsort/quicksortRecur(II)V
Lf:
    return
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 0
    putstatic qsort/i I
    ldc 0
    putstatic qsort/j I
    ldc 0
    putstatic qsort/tmp I
    ldc 0
    putstatic qsort/size I
    ldc 50
    multianewarray [I 1
    putstatic qsort/arr [I
    ldc 1
    putstatic qsort/i I
L31:
    getstatic qsort/i I
    getstatic qsort/size I
    if_icmple L32
    goto L33
L32:
    getstatic qsort/arr [I
    iaload
    invokestatic io/writelnI(I)V
    ldc 1
    getstatic qsort/i I
    iadd
    putstatic qsort/i I
    goto L31
L33:
    return
.end method
