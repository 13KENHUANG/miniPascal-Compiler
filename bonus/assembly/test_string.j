.class public test_string
.super java/lang/Object
.field public static s Ljava/lang/String;
.method public static main([Ljava/lang/String;)V
.limit locals 50
.limit stack 50
    ldc ""
    putstatic test_string/s Ljava/lang/String;
    ldc "rabbit"
    putstatic test_string/s Ljava/lang/String;
new java/lang/StringBuffer
dup
invokespecial java/lang/StringBuffer/<init>()V
    ldc "hello world "
invokevirtual java/lang/StringBuffer/append(Ljava/lang/String;)Ljava/lang/StringBuffer;
    aload s
invokevirtual java/lang/StringBuffer/append(Ljava/lang/String;)Ljava/lang/StringBuffer;
invokevirtual java/lang/StringBuffer/toString()Ljava/lang/String;
    invokestatic io/writelnS(Ljava/lang/String;)V
    return
.end method
