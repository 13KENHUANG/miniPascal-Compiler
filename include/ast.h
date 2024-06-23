#ifndef __AST_H__
#define __AST_H__


#ifdef __cplusplus
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <list>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <stack>
#include <algorithm>
#endif


#include "loc.h"
#include <stdint.h>
#include "typemap.h"
#include "sbt _define.h"



/* It's just for reference,
 * you can design your own
 * struct or class to impl inheritance in c/cpp */
 
using namespace std;

/*1-10*/
enum DeclarationKind {
	ROUTINEHEAD = 1,
	FUNCTION_Tree,
	FUNCTIONHEAD,
	PROCEDURE_Tree,
	PROCEDUREHEAD,
	DK_CONST,
	VAR_Tree,
	TYPE,
	VAR_PARA,
	VAL_PARA
};

/*11-16*/
enum ExpressionKind {
    EK_ID = 11,
    EK_CONST,
    OP,
    EK_CASE,
    FUNC_ID,
    FUNC_SYS
};

/*17-26*/
enum ExpressionType {
    VOID = 17,
    INT,
    REAL_Tree,
    CHAR,
    STRING_Tree,
    BOOL_Tree,
    ET_ARRAY,
    ET_RECORD,
    ET_SIMPLE_ENUM,
    ET_SIMPLE_LIMIT
};
/*27-56*/
enum OperationKind {
    TO = 27,
    DOWNTO,
    READ,
    WRITE,
    WRITELN,
    PLUS,
    MINUS,
    MUL,
    DIV,
    MOD,
    LT,
    LE,
    GT,
    GE,
    EQUAL,
    UNEQUAL, 
    NOT_Tree, 
    OR_Tree, 
    AND_Tree, 
    CHR, 
    ABS, 
    ODD, 
    ORD, 
    SQRT, 
    SUCC, 
    PRED, 
    SQR, 
    OK_ID, 
    OK_RECORD, 
    OK_ARRAY
};

/*57-67*/
enum StatementKind {
    LABEL = 57,
    ASSIGN,
    GOTO,
    IF_Tree,
    REPEAT,
    WHILE_Tree,
    FOR,
    SK_CASE,
    PROC_ID,
    PROC_FUNC
};

/*67-72*/
enum TypeKind {
    SIMPLE_SYS = 67,
    SIMPLE_ID,
    TK_SIMPLE_ENUM,
    TK_SIMPLE_LIMIT,
    TK_ARRAY,
    TK_RECORD
};

typedef class TreeDefine *TreeNode;

//-----symbol table-------//



struct Object
{
    int flag;
    int _int;
    double _double;
    string _string;

    list<struct Object> lo;//6 record
};



class TreeDefine
{
    private:
    vector<TreeNode> childNode;
    TreeNode siblingNode = NULL;
    int nodeType = 0;//init value
    Object attribute;
    ExpressionType expressionType;
    ExpressionType runtimeType;
    int lineNumber = 0;
    int columnNumber = 0;
    bitset<32> bitSet;
    
    struct OP_element
    {
        string op_type;
        int col;
        int row;
    };

    struct P_element
    {
        string OP;
        int level;
    };

    struct E_element
    {
        string et;
        int level;
    };

    static bool compareByLevel_E(const E_element &a, const E_element &b)
    {
        return a.level < b.level;
    }

    public:
    TreeDefine() {}

    TreeDefine(ExpressionKind nodeType, int lineNumber,int col):nodeType(nodeType),lineNumber(lineNumber),columnNumber(col){ this->expressionType = VOID;}

    TreeDefine(DeclarationKind nodeType, int lineNumber,int col):nodeType(nodeType),lineNumber(lineNumber),columnNumber(col) {}

    TreeDefine(TypeKind nodeType, int lineNumber,int col):nodeType(nodeType),lineNumber(lineNumber),columnNumber(col) {}

    TreeDefine(StatementKind nodeType, int lineNumber,int col):nodeType(nodeType),lineNumber(lineNumber),columnNumber(col) {}

    TreeDefine(TreeNode first, TreeNode second, OperationKind op, int lineNumber,int col) {
  		this->nodeType = OP;
		this->attribute._int = op;
		this->attribute.flag = 4;//expression type
		this->lineNumber = lineNumber;
		this->columnNumber = col;
		this->addChild(first);
		this->addChild(second);
    }

    TreeDefine(OperationKind op, TreeNode child, int lineNumber,int col) {
        this->nodeType = FUNC_SYS;
        this->attribute._int = op;
        this->attribute.flag = 4;//type
        this->lineNumber = lineNumber;
        this->columnNumber = col;
        this->addChild(child);
    }

    vector<TreeNode> getChildren();
    void addChild(TreeNode node);

    TreeNode getSibling();
    void setSibling(TreeNode siblingNode);

    int getLineNumber();
    void setLineNumber(int lineNumber);
    
    int getColNumber();
    void setColNumber(int colNumber);

    int getNodeType();
    void setNodeType(int nodeType);

    Object getAttribute();
    void setAttribute(Object attribute);
    void setAttribute(int attribute,int flag);
    void setAttribute(double attribute);
    void setAttribute(string attribute);

    ExpressionType getExpType();
    void setExpType(ExpressionType expressionType);

    ExpressionType getRunTimeType();
    void setRunTimeType(ExpressionType runtimeType);

    void printTree(TreeNode root, int level);
    
    void print_K(TreeNode root,int level);
    
    void Print_Test(TreeNode root);
    
    //----Main Function----
    void theCreate(TreeNode root);  
 

    void Sub_Var_decl(MySymbolTable &subTable,TreeNode root,int ,vector<string> &subShow);
    
    void FunPro_decl(MySymbolTable &symbolTable_Q,TreeNode root,int );
    

    vector<string> Spilt_type(string type);

    void Label_Check_Name(TreeNode label_node,string fun_name,bool &has_return);

    void New_Label_Check(MySymbolTable &myTable,TreeNode root,bool suc_defunc=true,int ROW=0,int COL=0,int IF_index=0,string func_name="");

    void New_OP(MySymbolTable &myTable,TreeNode root,bool &check);

   

    string Return_OP_type(TreeNode root);

    void  Check_OP_type(MySymbolTable &myTable,TreeNode root,vector<OP_element> &my_OP, vector<string> &soldier);

    void  Find_Fun_Para(MySymbolTable &myTable,TreeNode ch2,vector<string> &check_para,vector<string> right_type);

    void  Check_Array_Index(MySymbolTable &myTable,TreeNode root,bool &check);

    void Find_Fun_jj(MySymbolTable &myTable,TreeNode root);

    

    void  Get_Array_Index(MySymbolTable &myTable,TreeNode root,vector<string> &compare_data);

    

    void Check_fake_func_para(MySymbolTable &myTable,TreeNode root);

    void Check_IF_statement(MySymbolTable &myTable,TreeNode root,bool &check);

    //====for codegen====//
    void CreateBasicFun();

    void WRITEandREAD(MySymbolTable &myTable,TreeNode node);

    string WR_name(string name);

    void ShowLDC(string exp_type,TreeNode ch,int &isflaot);

    void Op_ss(string optype,int &BlockCount,bool Notfault=false);

    void Show_data_codegen(MySymbolTable K_symbolTable,vector<string> ctr);

    string Return_F_Name(string name,string type);

    string Return_Array_type(string name,string type);

    void  DealwithArraygen(MySymbolTable &myTable,TreeNode left_node,TreeNode right_node);

    void  Codegen_OP(MySymbolTable &myTable,TreeNode root,vector<string> &OP_Type, vector<string> &et,string &pp);

    void  Codegen_OP_2(MySymbolTable &myTable,TreeNode root,vector<P_element> &OP, vector<E_element> &et,int level);

    void  Return_F_para(MySymbolTable &myTable,TreeNode root,string &output);  

    void  Deal_with_Array(MySymbolTable &myTable,TreeNode root);
};


typedef enum{ ProgNode, /* types */ } NodeType;

typedef struct NodeTag{
  NodeType nt;
  LocType  loc;
  /* some fields */
} *Node, NodeStr;

#define Obj void*

typedef struct ConsTag{
  Obj car;
  struct ConsTag *cdr;
} *Cons, ConsStr;

#endif
