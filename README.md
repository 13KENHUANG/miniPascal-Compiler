# miniPascal Compiler implement
*A pascal compiler which can translate pascal code to Jasmin assembly language*  
**-->本專案以C/C++高級語言撰寫，將miniPascal語言作為編譯目標，最終生成能運行於JVM上的Java字節碼（Java bytecode)**  
**-->由於直接翻譯成Java字節碼，因此省略了中間代碼生成的過程**

# OverView
*要完成一個編譯器,通常需要幾個階段：
1. (O)詞法分析 (Lexical Analysis)
2. (O)語法分析 (Syntax Analysis)
3. (O)語義分析 (Semantic Analysis)
4. (X)中間代碼生成 (Intermediate Code Generation)
5. (X)中間代碼優化 (Intermediate Code Optimization)
6. (O)目標代碼生成 (Target Code Generation)

## 1.詞法分析 (Lexical Analysis)
目標：將源代碼轉換為一系列標記（tokens)。 
過程：讀取源代碼字符流，識別並生成標記，如關鍵字、標識符、操作符、字面值等。  
工具：可以使用工具如 Lex 或 Flex 來生成詞法分析器。  

## 2.語法分析 (Syntax Analysis)

## 3.語義分析 (Semantic Analysis)

## 4.目標代碼生成 (Target Code Generation)

## Runtime Environment
* Ubuntu 20.04.4
* G++ 9.4
* Flex 2.6.4
* Bison 3.5.1
## Functionality implementation
* Scanning. Using Lex
* Parsing. Using Yacc
* Abstract Syntax Tree
* Symbol Table
* Semantic Analysis
* Code generation Target: Jasmin
* Error detection
  
## Support Type
* Integer、 Real、 Char、 Array
* Statement: if else、while
* Arithmetic Operations
* Global Variable
* Function
* Procedure

## Summary and flow chart

![This is an alt text.](minipascal.PNG)
