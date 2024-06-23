.class public test_array_slice
.super java/lang/Object
.field public static a [[I
.method public static slice(I)I
.limit locals 50
.limit stack 50
    iload_0
    iload_0
    iadd
    iload_0
    iadd
Lf:
    ireturn
.end method
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc 2
    ldc 5
    multianewarray [[I 2
    putstatic test_array_slice/a [[I
    getstatic test_array_slice/a [[I
    ldc 2
    ldc 1
    isub
    aaload
    ldc 3
    ldc 1
    isub
    ldc 1
    iastore
    getstatic test_array_slice/a [[I
    ldc 2
    ldc 1
    isub
    aaload
    ldc 4
    ldc 1
    isub
    ldc 2
    iastore
    getstatic test_array_slice/a [[I
    ldc 2
    ldc 1
    isub
    aaload
    ldc 5
    ldc 1
    isub
    ldc 3
    iastore
    invokestatic test_array_slice/slice(I)I
    invokestatic io/writelnI(I)V
    return
.end method
