.class public test_array_assignment
.super java/lang/Object
.field public static a [I
.field public static b I
.field public static c [[I
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 5
    multianewarray [I 1
    putstatic test_array_assignment/a [I
    ldc 0
    putstatic test_array_assignment/b I
    ldc 2
    ldc 5
    multianewarray [[I 2
    putstatic test_array_assignment/c [[I
    ldc "a[1..5]:"
    invokestatic io/writelnS(Ljava/lang/String;)V
    ldc 1
    putstatic test_array_assignment/b I
L1:
    getstatic test_array_assignment/b I
    ldc 5
    if_icmple L2
    goto L3
L2:
    getstatic test_array_assignment/a [I
    ldc 1
    getstatic test_array_assignment/b I
    iadd
    iastore
    getstatic test_array_assignment/a [I
    iaload
    invokestatic io/writelnI(I)V
    ldc 1
    getstatic test_array_assignment/b I
    iadd
    putstatic test_array_assignment/b I
    goto L1
L3:
    ldc "c[1][1..5]:"
    invokestatic io/writelnS(Ljava/lang/String;)V
    ldc 1
    putstatic test_array_assignment/b I
L3:
    getstatic test_array_assignment/b I
    ldc 5
    if_icmple L4
    goto L5
L4:
    getstatic test_array_assignment/c [[I
    ldc 1
    ldc 1
    isub
    iaload
    invokestatic io/writelnI(I)V
    ldc 1
    getstatic test_array_assignment/b I
    iadd
    putstatic test_array_assignment/b I
    goto L3
L5:
    ldc "c[1] := a[1..5];"
    invokestatic io/writelnS(Ljava/lang/String;)V
    getstatic test_array_assignment/c [[I
    ldc 1
    ldc 1
    isub
    getstatic test_array_assignment/a [I
    iaload
    iastore
    ldc "c[1][1..5]:"
    invokestatic io/writelnS(Ljava/lang/String;)V
    ldc 1
    putstatic test_array_assignment/b I
L5:
    getstatic test_array_assignment/b I
    ldc 5
    if_icmple L6
    goto L7
L6:
    getstatic test_array_assignment/c [[I
    ldc 1
    ldc 1
    isub
    iaload
    invokestatic io/writelnI(I)V
    ldc 1
    getstatic test_array_assignment/b I
    iadd
    putstatic test_array_assignment/b I
    goto L5
L7:
    ldc "a[1] := 100;"
    invokestatic io/writelnS(Ljava/lang/String;)V
    getstatic test_array_assignment/a [I
    ldc 1
    ldc 1
    isub
    ldc 100
    iastore
    ldc "c[1][1..5]:"
    invokestatic io/writelnS(Ljava/lang/String;)V
    ldc 1
    putstatic test_array_assignment/b I
L7:
    getstatic test_array_assignment/b I
    ldc 5
    if_icmple L8
    goto L9
L8:
    getstatic test_array_assignment/c [[I
    ldc 1
    ldc 1
    isub
    iaload
    invokestatic io/writelnI(I)V
    ldc 1
    getstatic test_array_assignment/b I
    iadd
    putstatic test_array_assignment/b I
    goto L7
L9:
    return
.end method
