# miniPascal Compiler implement
*A pascal compiler which can translate pascal code to Jasmin assembly language*
**本專案以C/C++高級語言撰寫，並將miniPascal語言作為編譯目標，最終生成能運行於JVM上的Java字節碼（Java bytecode)**

# OverView
*要完成一個編譯器,通常需要幾個階段：
1. 詞法分析 (Lexical Analysis)
*目標：將源代碼轉換為一系列標記（tokens）。*
*過程：讀取源代碼字符流，識別並生成標記，如關鍵字、標識符、操作符、字面值等。*
*工具：可以使用工具如 Lex 或 Flex 來生成詞法分析器。*

2. 語法分析 (Syntax Analysis)
    目標：將標記序列轉換為語法樹（Syntax Tree）。
    過程：根據語法規則（通常使用上下文無關語法）解析標記序列，構建語法樹。
    工具：可以使用工具如 Yacc 或 Bison 來生成語法分析器。

3. 語義分析 (Semantic Analysis)
    目標：檢查語法樹是否符合語言的語義規則，並收集必要的信息（如符號表）。
    過程：進行類型檢查、作用域解析、變量定義和使用等語義檢查。

4. 中間代碼生成 (Intermediate Code Generation)
    目標：將語法樹轉換為中間表示（Intermediate Representation，IR），如三地址碼（Three-Address Code）。
    過程：生成一種介於高級語言和機器碼之間的中間代碼，便於後續的優化和代碼生成。

5. 中間代碼優化 (Intermediate Code Optimization)
    目標：提高中間代碼的執行效率和減少資源使用。
    過程：應用各種優化技術，如常量折疊、死代碼消除、循環優化等。

6. 目標代碼生成 (Target Code Generation)
    目標：將優化後的中間代碼轉換為目標機器的低級表示，如匯編語言或機器碼。
    過程：根據目標機器的指令集生成對應的低級代碼。


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
