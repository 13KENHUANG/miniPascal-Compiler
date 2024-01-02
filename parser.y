%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <cstring>


#include "loc.h"
#include "ast.h"
#include "error.h"
#include "info.h"
using namespace std;

#define YYLTYPE LocType

#define MAX_LINE_LENG      256
extern int line_no, col_no, opt_list;
extern char buffer[MAX_LINE_LENG];
extern FILE *yyin;        /* declared by lex */
extern char *yytext;      /* declared by lex */
extern int yyleng;

int pass_error = 0;
char *output = NULL;

//static Node root = NULL;
TreeNode savedTree=NULL;

extern
#ifdef __cplusplus
"C"
#endif
int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);

%}

%locations

%token PROGRAM VAR ARRAY OF INTEGER REAL STRING PROCEDURE PBEGIN END IF THEN ELSE WHILE DO NOT AND OR

%token LPAREN RPAREN SEMICOLON DOT COMMA COLON LBRACE RBRACE DOTDOT ASSIGNMENT 

%token<val>  INTEGERNUM  
%token<text> LITERALSTR 
%token<dval> REALNUMBER SCIENTIFIC 
%token<loc>  FUNCTION IDENTIFIER ADDOP SUBOP MULOP DIVOP LTOP GTOP EQOP GETOP LETOP NEQOP

%token T_CONST T_TYPE T_RECORD T_GOTO T_REPEAT T_UNTIL T_CASE T_FOR T_TO  T_DOWNTO T_READ T_WRITE T_WRITELN T_UNEQUAL T_MOD

%union {
  int val;
  char* text;
  double dval;
  TreeNode node;
  LocType loc;
}

%type <node> prog routine routine_head routine_part function_decl function_head parameters para_decl_list para_type_list
%type <node> procedure_decl procedure_head var_part var_decl_list var_decl    
%type <node> type_decl  
%type <node> simple_type_decl name_list ID routine_body compound_stmt stmt_list  assign_stmt  if_stmt else_clause 
%type <node> term factor
%type <node> while_stmt expression stmt stmt2 tail

//---add----
%type <node> array_type_decl const_value  stmt_list_list procedure_stmt expression_list 
%type <node> boolexpression simple_expression 

%%

    /* define your snytax here */
    /* @n return the sturct LocType of "n-th node", ex: @1 return the PROGRAM node's locType
       $n return the $$ result you assigned to the rule, ex: $1 */



















prog: PROGRAM IDENTIFIER LPAREN identifier_list_p RPAREN SEMICOLON  
      routine
      DOT
      {        
      	$$ = $7;
      	$$->setAttribute($2.text);
      	savedTree = $$;
	
      }
      ;
      
identifier_list_p: IDENTIFIER
	        | identifier_list_p COMMA IDENTIFIER   //id,id
	        ;
	
routine: routine_head routine_body
	{
	//   cout<<"routine\n";
	   $$=$1;
	   //$$->setSibling($2);
	   $$->addChild($2);
	}
	;

routine_head:  var_part routine_part
	     {
	   //     cout<<"routine_head\n";
	       $$ = new TreeDefine(ROUTINEHEAD,line_no,col_no-yyleng);
	       $$->addChild($1);
                $$->addChild($2);
                //  $$->addChild($3);
	     }
	     ;

routine_part:  {$$ = NULL;}
	    | routine_part procedure_decl	
	      {
	        TreeNode t = $1;
	        if(t!=NULL){
	        	  while(t->getSibling()!=NULL)
	        	     {
	      //  	       cout<<"routine_part procedure_decl\n";
	        	       t = t->getSibling();
	        	     }
	        	  t->setSibling($2);
	        	  $$=$1;
	        }
	        else
	        	  $$=$2;
	      }
	    | routine_part function_decl
	      {
	       TreeNode t = $1;
	        if(t!=NULL){
	        	  while(t->getSibling()!=NULL)
	        	     {
	     //   	       cout<<"routine_part function_decl\n";
	        	       t = t->getSibling();
	        	     }
	        	  t->setSibling($2);
	        	  $$=$1;
	        }
	        else
	        	  $$=$2;
	      }
	    | procedure_decl 
	    {
	//      cout<<"procedure_decl\n";
	      $$=$1;
	    }
	    | function_decl  
	    {
	//      cout<<"function_decl\n";
	      $$=$1;
	    }
	    ;
	    
function_decl: function_head SEMICOLON routine SEMICOLON
	      {
	     //     cout<<"function_decl\n";
	      	$$=new TreeDefine(FUNCTION_Tree,line_no,col_no-yyleng);
	      	$$->addChild($1);
	      	$$->addChild($3);
	      }
	      ;
	      
function_head: FUNCTION IDENTIFIER parameters  COLON simple_type_decl
	      {
	  //        cout<<"function_head\n";
	        $$=new TreeDefine(FUNCTIONHEAD,$1.first_line,$1.first_column);
	        $$->setAttribute($2.text);
	        $$->addChild($3);
                 $$->addChild($5);

	      }
	      ;

parameters:  {$$ = NULL;}
	  | LPAREN para_decl_list RPAREN
	    {
	  //   cout<<"parameters\n";
	     $$=$2;
	    }
	    ;
	  
para_decl_list: para_decl_list  SEMICOLON  para_type_list
	       {
	  //      cout<<"para_decl_list_1\n";
	        TreeNode t = $1;
	        if(t!=NULL){
                                while(t->getSibling()!=NULL)
                                  t=t->getSibling();
                                t->setSibling($3);
                                $$=$1;
                            }
                  else
                  	$$=$3;
	       }
	      | para_type_list
	      {
	  //      cout<<"para_decl_list_2\n";
	        $$=$1;
	      }
	      ;
	      
para_type_list: VAR name_list COLON type_decl
	       {
	    //      cout<<"para_type_list_1\n";
	       	$$=new TreeDefine(VAR_PARA,line_no,col_no-yyleng);
	       	$$->addChild($2);
	       	$$->addChild($4);
	       }
	      | name_list COLON type_decl
	        {
	   //      cout<<"para_type_list_2\n";
	         $$=new TreeDefine(VAL_PARA,line_no,col_no-yyleng);
	         $$->addChild($1);
	       	$$->addChild($3);
	        }
	      
	      ;

	      
procedure_decl :  procedure_head SEMICOLON routine SEMICOLON
		{
	//	   cout<<"procedure_decl\n";
		   $$=new TreeDefine(PROCEDURE_Tree,line_no,col_no-yyleng);
		   $$->addChild($1);
		   $$->addChild($3);
		}
		;

procedure_head:  PROCEDURE IDENTIFIER parameters
		{
	//	cout<<"procedure_head\n";
		$$=new TreeDefine(PROCEDUREHEAD,line_no,col_no-yyleng);
                  $$->setAttribute($2.text);
                  $$->addChild($3);
		}
		;

var_part:  {
         //  cout<<"var_part_1\n";
           $$ = NULL;
           }
	| var_part VAR var_decl_list
	  {
	//   cout<<"var_part_2\n";
	   TreeNode t = $1;
	        if(t!=NULL){
                                while(t->getSibling()!=NULL)
                                  t=t->getSibling();
                                t->setSibling($3);
                                $$=$1;
                            }
                  else
                  	$$=$3;
	  }
	;
	
var_decl_list :  var_decl
                {
         //       cout<<"var_decl_list\n";
	      $$=$1;
	      };
	
	     
var_decl: name_list COLON type_decl SEMICOLON
	{
	//   cout<<"var_decl\n";
	   $$=new TreeDefine(VAR_Tree,line_no,col_no-yyleng);
	   $$->addChild($1);
	   $$->addChild($3);
	}
	;
                   

 
type_decl :  simple_type_decl  
              {
            //  cout<<"type_decl_1\n";
	     $$=$1;
	    }
	   | array_type_decl  
	   {
	//    cout<<"type_decl_2\n";
	    $$=$1;
	   }
 	    ;
 	    
const_value:  INTEGERNUM
	     {
	  //      cout<<"const_value\n";
	        $$ =new TreeDefine(EK_CONST,line_no,col_no-yyleng);
                  $$->setExpType(INT);
                  $$->setAttribute($1);
	     }
	     ;

array_type_decl: ARRAY LBRACE simple_type_decl RBRACE OF type_decl
	       {
	    //      cout<<"array_type_decl\n";
	       	$$=new TreeDefine(TK_ARRAY,line_no,col_no-yyleng);
	       	$$->addChild($3);
	       	$$->addChild($6);
	       	$$->setExpType(ET_ARRAY);
	       }
	       ;

simple_type_decl: INTEGER
	        {
	      //   cout<<"INTEGER\n";
	         $$=new TreeDefine(SIMPLE_SYS,line_no,col_no-yyleng);
                  $$->setExpType(INT);
	        }
	       | REAL
	        {
	    //     cout<<"REAL\n";
	         $$=new TreeDefine(SIMPLE_SYS,line_no,col_no-yyleng);
                  $$->setExpType(REAL_Tree);
	        }
	       | STRING
	        {
	    //      cout<<"STRING\n";
	          $$=new TreeDefine(SIMPLE_SYS,line_no,col_no-yyleng);
                   $$->setExpType(CHAR);
	        }
	       | const_value DOTDOT const_value
	        {
	  //       cout<<"const_value DOTDOT const_value\n";
	        	$$=new TreeDefine(TK_SIMPLE_LIMIT,line_no,col_no-yyleng);
                    $$->addChild($1);
                    $$->addChild($3);
                    $$->setExpType(ET_SIMPLE_LIMIT);
	        }
	       ;
	       
name_list: name_list COMMA ID
	 {
	//   cout<<"name_list COMMA ID\n";
	   TreeNode t=$1;
             if(t!=NULL){
                while(t->getSibling()!=NULL)
                      t=t->getSibling();
                t->setSibling($3);
                $$=$1;
                        }
            else
               $$=$3;
               
	 }
	 | ID 
	  {
//	  cout<<"name_list : ID\n";
	  $$=$1;
	  };
	 
ID:  IDENTIFIER
     {
   //   cout<<"IDENTIFIER\n";
      $$=new TreeDefine(EK_ID,$1.first_line,$1.first_column);
      $$->setAttribute($1.text);
     }
     ;

routine_body :  compound_stmt{
          //          cout<<"routine_body :  compound_stmt\n";
		$$=$1;
		};

compound_stmt: PBEGIN 
	      stmt_list 
               {
        //       cout<<"compound_stmt: PBEGIN \n";
               $$=$2;
               }
               ;
               
stmt_list : { $$=NULL;}
	  |stmt_list stmt_list_list END
	  {
	   TreeNode t=$1;
             if(t!=NULL){
                while(t->getSibling()!=NULL)
                      t=t->getSibling();
                t->setSibling($2);
                $$=$1;
                        }
            else
               $$=$2;
               
               
            
	  }
	  |END
	  {
	    $$=NULL;
	  }
	  ;
	 
stmt_list_list:stmt2 {$$=$1;}
	    | stmt_list_list SEMICOLON  stmt2
	    {
	   TreeNode t=$1;
             if(t!=NULL){
                while(t->getSibling()!=NULL)
                      t=t->getSibling();
                t->setSibling($3);
                $$=$1;
                        }
            else
               $$=$3;
	    }
	    ;
	  
stmt2: stmt
	{
	  $$=new TreeDefine(LABEL,line_no,col_no-yyleng);
	  $$->addChild($1);
	}
	;
      
stmt:     assign_stmt   {$$=$1;}
	|procedure_stmt{$$=$1;}
	|compound_stmt {$$=$1;}
	|if_stmt       {$$=$1;}
	|while_stmt    {$$=$1;}
 	|              {$$=NULL;}
 	;    
 	

assign_stmt : ID ASSIGNMENT expression
	     {
	         $$=new TreeDefine(ASSIGN,$1->getLineNumber(),$1->getColNumber());
	         $$->addChild($1);
                   $$->addChild($3);
                   $$->setAttribute(OK_ID,4);
	     }
	     |ID tail ASSIGNMENT expression
	     {
	     	$$=new TreeDefine(ASSIGN,$1->getLineNumber(),$1->getColNumber());
	     	$$->addChild($1);
	     	if(!$$->getChildren().empty())
	     	 ($$->getChildren().at(0))->addChild($2);
	     	
	     	else 
	     	  $$->addChild($2);
	     	$$->addChild($4);
	     	$$->setAttribute(OK_ARRAY,4);
	     }
	     ;

tail : LBRACE expression RBRACE tail
      {
        $$=$2;
        $$->setSibling($4);
       // $$->addChild($4);
      }
      |{$$=NULL;}
      ;
	   
if_stmt:	  IF expression THEN stmt else_clause
	   {
	      $$=new TreeDefine(IF_Tree,line_no,col_no-yyleng);
	      $$->addChild($2);	
	      $$->addChild($4);
	      $$->addChild($5);
	   }
	   ;

else_clause : {$$=NULL;}
	   | ELSE stmt{$$=$2;};

while_stmt: WHILE expression DO stmt
	   {
	     $$=new TreeDefine(WHILE_Tree,$2->getLineNumber(),$2->getColNumber());
	     $$->addChild($2);
	     $$->addChild($4);
	   };
  

     
procedure_stmt:ID
	   {
	    $$=new TreeDefine(PROC_ID,$1->getLineNumber(),$1->getColNumber());
	    $$->setAttribute($1->getAttribute());	
	   }
	| ID LPAREN expression_list RPAREN
	  {
	    $$=new TreeDefine(PROC_ID,$1->getLineNumber(),$1->getColNumber());
	    $$->setAttribute($1->getAttribute());
	    $$->addChild($3);
	  }
	;
	
expression_list : expression {$$=$1;}
	       | expression_list COMMA expression
	        {
	         TreeNode t=$1;
                   if(t!=NULL){
                     while(t->getSibling()!=NULL)
                          t=t->getSibling();
                     t->setSibling($3);
                     $$=$1;
                        }
                  else
                    $$=$3;
	        }
	       ;
	       
	       
	       
expression : boolexpression { $$=$1;}
	 | boolexpression AND boolexpression
	   {
	 //   cout<<"boolexpression AND boolexpression\n";
	    $$=new TreeDefine($1,$3,AND_Tree,line_no,col_no-yyleng);
	   }
	 | boolexpression OR boolexpression
	   {
	 //   cout<<"boolexpression OR boolexpression\n";
	    $$=new TreeDefine($1,$3,OR_Tree,line_no,col_no-yyleng);
	   }
	 ;	
	 
boolexpression : simple_expression { $$=$1;}
	      | simple_expression GTOP simple_expression
	      {
	     //  cout<<"simple_expression GTOP simple_expression\n";
	        $$=new TreeDefine($1,$3,GT,$2.first_line,$2.first_column);
	       }
	      | simple_expression LTOP simple_expression
	      {
	    //   cout<<"simple_expression LTOP simple_expression\n";
	        $$=new TreeDefine($1,$3,LT,$2.first_line,$2.first_column);
	       }
	      | simple_expression EQOP simple_expression
	      {
	    //   cout<<"simple_expression EQOP simple_expression\n";
	        $$=new TreeDefine($1,$3,EQUAL,$2.first_line,$2.first_column);
	       }
	      | simple_expression GETOP simple_expression
	      {
	    //   cout<<"simple_expression GETOP simple_expression\n";
	        $$=new TreeDefine($1,$3,GE,$2.first_line,$2.first_column);
	       }
	      | simple_expression LETOP simple_expression
	      {
	  //     cout<<"simple_expression LETOP simple_expression\n";
	        $$=new TreeDefine($1,$3,LE,$2.first_line,$2.first_column);
	       }
	      | simple_expression NEQOP simple_expression
	      {
	//      cout<<"simple_expression NEQOP simple_expression\n";
	        $$=new TreeDefine($1,$3,UNEQUAL,$2.first_line,$2.first_column);
	       }
	      ;

/*relop : GTOP   {$$=GT;} 
       | LTOP  {$$=LT;} 
       | EQOP  {$$=EQUAL;} 
       | GETOP {$$=GE;} 
       | LETOP {$$=LE;} 
       | NEQOP {$$=UNEQUAL;} 
       ;  */
	      
simple_expression : term { $$=$1;}
		| simple_expression ADDOP term
		{
	//	 cout<<"simple_expression ADDOP term\n";
		  $$=new TreeDefine($1,$3,PLUS,$2.first_line,$2.first_column);
		}
		| simple_expression SUBOP term
		{
	//	 cout<<"simple_expression SUBOP term\n";
		  $$=new TreeDefine($1,$3,MINUS,$2.first_line,$2.first_column);
		}
		;	 

/*addop : ADDOP {$$=PLUS;} 
      | SUBOP {$$=MINUS;}
      ;
      */


term : factor { $$=$1;}
      | term MULOP factor
      {
    //  cout<<"term MULOP factor\n";
       $$=new TreeDefine($1,$3,MUL,$2.first_line,$2.first_column);
      }
      | term DIVOP factor
      {
    //  cout<<"term DIVOP factor\n";
       $$=new TreeDefine($1,$3,DIV,$2.first_line,$2.first_column);
      }
     ;
   
/*mulop : MULOP {$$=MUL;} 
      | DIVOP {$$=PDIV;} 
      ;*/
  
factor : ID tail
         {
       //   cout<<"ID tail\n";
         	 $$=new TreeDefine(PROC_ID,$1->getLineNumber(),$1->getColNumber());
         	 $$->setAttribute($1->getAttribute());
         	 if(!$$->getChildren().empty())
         	   ($$->getChildren().at(0))->addChild($2);
       
         	 else
         	    $$->addChild($2);
         }
       | ID LPAREN expression_list RPAREN
         {
       //   cout<<"ID LPAREN expression_list RPAREN\n";
           $$=new TreeDefine(PROC_FUNC,$1->getLineNumber(),$1->getColNumber());
           $$->setAttribute($1->getAttribute());
           $$->addChild($3);
         }
       | INTEGERNUM
         {
         //  cout<<"Hello INTEGERNUM="<<$1<<"\n";
          $$ =new TreeDefine(EK_CONST,line_no,col_no-yyleng);
          $$->setExpType(INT);
          $$->setAttribute($1);
         }
       | REALNUMBER
         {
       //   cout<<"REALNUMBER\n";
          $$ = new TreeDefine(EK_CONST,line_no,col_no-yyleng);
          $$->setExpType(REAL_Tree);
          $$->setAttribute($1);
         }
       | LITERALSTR
         {
      //    cout<<"LITERALSTR\n";
          $$ = new TreeDefine(EK_CONST,line_no,col_no-yyleng);
          $$->setExpType(STRING_Tree);
          $$->setAttribute($1);
         }         
       | LPAREN expression RPAREN {$$=$2;}
       | SUBOP factor   
       {
     //  cout<<"SUBOP factor\n";
       $$=new TreeDefine($2,NULL,MINUS,line_no,col_no-yyleng);
       }
       | NOT factor     
       {
     //  cout<<"NOT factor\n";
       $$=new TreeDefine($2,NULL,NOT_Tree,line_no,col_no-yyleng);
       }
       ;

%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "[ERROR] line %4d:%3d %s, Unmatched token: %s\n",
            line_no, col_no - yyleng, buffer, yytext);
    pass_error = 1;
}

void codegen(TreeNode savedTree,string currentDir) {
   if(savedTree==NULL) return;
   
   char fn[128];
   sprintf(fn, "%s.j", savedTree->getAttribute()._string.c_str());
   currentDir+=fn;
   
   FILE *fd = fopen(currentDir.c_str(), "w");
   if (fd == NULL) {
        perror("Error opening file for code generation");
        exit(1);
    }
   string tmp=".super java/lang/Object\n";
   fprintf(fd,"%s",tmp.c_str());
   //savedTree->theCreate(savedTree,fd);
   

   
   fclose(fd);
}

int main(int argc, char *argv[]) {

    char c;
    while((c=getopt(argc, argv, "o:")) != -1){
      switch(c){
        case 'o':
          output = optarg;
          break;
        case '?':
            fprintf(stderr, "Illegal option:-%c\n", isprint(optopt)?optopt:'#');
            break;
        default:
            fprintf( stderr, "Usage: %s [-o output] filename\n", argv[0]), exit(0);
            break;
      }
    }

    FILE *fp = argc == 1 ? stdin : fopen(argv[optind], "r");

    if(fp == NULL)
        fprintf( stderr, "Open file error\n" ), exit(-1);

    yyin = fp;
    yyparse();
    
    //=========================//
    char currentDir[1024];
    string currentPath;
    if (getcwd(currentDir, sizeof(currentDir)) != NULL) 
    {
        currentPath = currentDir;
    }
    else
    {
       perror("getcwd");
       return 1;
    }
    string tmppath="/home/ken/Softwork/_Compiler_/HW4/hw4-main/test/assembly/fibonacci_recursive.j";
    currentPath+="/assembly/";
    
    
    if(!pass_error && savedTree!=NULL)
    {
         
    	string name = savedTree->getAttribute()._string;
    	currentPath+=name ;
    	currentPath+=".j";
    	FILE *fd = fopen(currentPath.c_str(), "w");
    	if (fd == NULL) 
    	{
        	   perror("Error opening file for code generation");
            return 0;
   	}
   	savedTree->theCreate(savedTree,fd);
   	/*string tmp=".super java/lang/Object\n";
    	fprintf(fd,"156987\n");
    	fprintf(fd,"%s",tmp.c_str());
    	fprintf(fd,"%s",currentPath.c_str());*/
    }
    
    
   
    
    


    //=========================//
    /*if(!pass_error && savedTree!=NULL){
       codegen(savedTree,currentPath);

      // do pass here

    }*/
    return 0;
}
