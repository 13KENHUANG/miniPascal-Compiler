%{
/*
 * scanner.l
 *
 * lex input file for pascal scanner
 *
 */

#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "parser.h"
#include "info.h"

int fileno(FILE *);

#define YY_USER_ACTION \
    yylloc.first_line = line_no; \
    yylloc.first_column = col_no; \
    col_no += yyleng;

#define MAX_LINE_LENG      256
#define LIST       strcat(buffer, yytext)
#define LIST_FLUSH do{ if(opt_list) printf("%s", buffer); *buffer = 0; }while(0)
#define LOG(TYPE) \
    do{ LIST; \
        if(opt_token) \
        fprintf(stderr, "token(type:%-10s) on line %4d, %3d : %s\n", \
            #TYPE, line_no, col_no - yyleng, yytext); \
    } while(0)

#ifdef __cplusplus
extern "C" int yylex(void);
#endif

#define C1_LOG(tmp)       
#define C2_LOG_Start(tmp) 
#define C2_LOG_End(S)   

int opt_list = 1, opt_token = 0;
int line_no = 1, col_no = 1;
char buffer[MAX_LINE_LENG];

int  comment_num=1;
int  comment_newline=0;
int  tmp=0;

%}

%option nounput
%option noinput

A [aA]
B [bB]
C [cC]
D [dD]
E [eE]
F [fF]
G [gG]
H [hH]
I [iI]
J [jJ]
K [kK]
L [lL]
M [mM]
N [nN]
O [oO]
P [pP]
Q [qQ]
R [rR]
S [sS]
T [tT]
U [uU]
V [vV]
W [wW]
X [xX]
Y [yY]
Z [zZ]

%x COMMENT
%x COMMENT_SINGLE

%%
                             /* v could do something */
{P}{R}{O}{G}{R}{A}{M}       {LOG(KEYWORD); return(PROGRAM);    }
{V}{A}{R}                   {LOG(KEYWORD); return(VAR);        }
{A}{R}{R}{A}{Y}             {LOG(KEYWORD); return(ARRAY);      }
{O}{F}                      {LOG(KEYWORD); return(OF);         }
{I}{N}{T}{E}{G}{E}{R}       {LOG(KEYWORD); return(INTEGER);    }
{R}{E}{A}{L}                {LOG(KEYWORD); return(REAL);       }
{S}{T}{R}{I}{N}{G}          {LOG(KEYWORD); return(STRING);     }
{F}{U}{N}{C}{T}{I}{O}{N}    {
			  yylval.loc.first_line = line_no;
   		           yylval.loc.first_column = col_no - yyleng;
			  LOG(KEYWORD); return(FUNCTION);   
		          }
{P}{R}{O}{C}{E}{D}{U}{R}{E} {LOG(KEYWORD); return(PROCEDURE);  }
{B}{E}{G}{I}{N}             {LOG(KEYWORD); return(PBEGIN);     }
{E}{N}{D}                   {LOG(KEYWORD); return(END);        }
{I}{F}                      {LOG(KEYWORD); return(IF);         }
{T}{H}{E}{N}                {LOG(KEYWORD); return(THEN);       }
{E}{L}{S}{E}                {LOG(KEYWORD); return(ELSE);       }
{W}{H}{I}{L}{E}             {LOG(KEYWORD); return(WHILE);      }
{D}{O}                      {LOG(KEYWORD); return(DO);         }
{N}{O}{T}                   {LOG(KEYWORD); return(NOT);        }
{A}{N}{D}                   {LOG(KEYWORD); return(AND);        }
{O}{R}                      {LOG(KEYWORD); return(OR);         }

"("                         {LOG(KEYWORD); return(LPAREN);     }
")"                         {LOG(KEYWORD); return(RPAREN);     }
";"                         {LOG(KEYWORD); return(SEMICOLON);  }
"."                         {LOG(KEYWORD); return(DOT);        }
","                         {LOG(KEYWORD); return(COMMA);      }
":"                         {LOG(KEYWORD); return(COLON);      }
"["                         {LOG(KEYWORD); return(LBRACE);     }
"]"                         {LOG(KEYWORD); return(RBRACE);     }
".."                        {LOG(KEYWORD); return(DOTDOT);     }
":="                        {LOG(KEYWORD); return(ASSIGNMENT); }
"+"                         {
			 yylval.loc.first_line = line_no;
   		           yylval.loc.first_column = col_no - yyleng;
			 LOG(KEYWORD); 
			 return(ADDOP);      
		        }
"-"                         {
			 yylval.loc.first_line = line_no;
   		           yylval.loc.first_column = col_no - yyleng;
			 LOG(KEYWORD); 
			 return(SUBOP);      
		        }
"*"                         {
			yylval.loc.first_line = line_no;
   		          yylval.loc.first_column = col_no - yyleng;
			LOG(KEYWORD); 
			return(MULOP);      
		        }
"/"                         {
			yylval.loc.first_line = line_no;
   		          yylval.loc.first_column = col_no - yyleng;
			LOG(KEYWORD); 
			return(DIVOP);      
		        }
">"                         {
			yylval.loc.first_line = line_no;
   		          yylval.loc.first_column = col_no - yyleng;
			LOG(KEYWORD); 
			return(GTOP);       
		        }
"<"                         {
			yylval.loc.first_line = line_no;
   		          yylval.loc.first_column = col_no - yyleng;
			LOG(KEYWORD); 
			return(LTOP);       
		        }
"="                         {
			yylval.loc.first_line = line_no;
   		          yylval.loc.first_column = col_no - yyleng;
			LOG(KEYWORD); 
			return(EQOP);       
		        }
">="                        {
			yylval.loc.first_line = line_no;
   		          yylval.loc.first_column = col_no - yyleng;
			LOG(KEYWORD); 
			return(GETOP);      
		        }
"<="                        {
			yylval.loc.first_line = line_no;
   		          yylval.loc.first_column = col_no - yyleng;
			LOG(KEYWORD); 
			return(LETOP);      
		        }
"!="                        {
			yylval.loc.first_line = line_no;
   		          yylval.loc.first_column = col_no - yyleng;
			LOG(KEYWORD); 
			return(NEQOP);      
		        }

  /* define identifier here */
[a-zA-Z][a-zA-Z0-9]*                  {
			            LIST;
				   yylval.loc.text = (char*)malloc(strlen(yytext)+1);
				   strcpy(yylval.loc.text,yytext);				   
				   yylval.loc.first_line = line_no;
   		          	   yylval.loc.first_column = col_no - yyleng;
				   return(IDENTIFIER);
			           }
[a-zA-Z]([a-zA-Z0-9_])+([a-zA-Z0-9])+ {
				   LIST;
                                       yylval.loc.text = (char*)malloc(strlen(yytext)+1);
				   strcpy(yylval.loc.text,yytext);
				   
				   yylval.loc.first_line = line_no;
   		          	   yylval.loc.first_column = col_no - yyleng;
				   return(IDENTIFIER);
			           }

  /* define INTEGERNUM, REALNUMBER, SCIENTIFIC here */
[0-9]+                        {
		             LIST;
		             // change to int
		             yylval.val=atoi(yytext);
		             return(INTEGERNUM);
		            }
[0-9]+\.[0-9]+                {
			    LIST;
			    // change to float
			    yylval.dval=atof(yytext);
			    return(REALNUMBER);
			   }
[0-9]+[Ee][+-]?[0-9]+         {
			    LIST;
			    //strtod為C語言的函式庫(轉換科學記號至浮點數),輸入字串須為const char*類型
			    yylval.dval=strtod(yytext,NULL); 
			    return(SCIENTIFIC);
			   }
[0-9]+\.[0-9]+[Ee][+-]?[0-9]+ {
		             LIST;
		             yylval.dval=strtod(yytext,NULL); 
		             return(SCIENTIFIC);
		            } 

  /* define single/multiple line comment here */
"/*"          {
	     tmp=col_no - yyleng;
	     LIST;
	     comment_num=3;
	     C2_LOG_Start(tmp);
	     BEGIN(COMMENT); 
	     }  // 进入 COMMENT 起始条件
<COMMENT>"*/" {
	     LIST;
	     C2_LOG_End(comment_num);
	     BEGIN(INITIAL);
	     }  // 退出 COMMENT 起始条件	 
<COMMENT>\n   {
	     LIST;
	     LIST_FLUSH;
	     comment_num=1;
	     line_no++;
	     col_no = 1;
	     }	      
<COMMENT>.    {
	     LIST;
	     comment_num++;
	     }  

"//"                 {
                     tmp=col_no - yyleng;
	            LIST;
	            BEGIN(COMMENT_SINGLE);
	            }
<COMMENT_SINGLE>"\n" {
	            LIST;
	            LIST_FLUSH;
	            C1_LOG(tmp);
	            BEGIN(INITIAL);
	            line_no++;
	            col_no = 1;
	            }
<COMMENT_SINGLE>.    {LIST;}

  /* define string constant (LITERALSTR) here */
\"[^\"]*\"          {
		  LOG(LITERALSTR);
		  //strdup 函数会在堆上复制 yytext 的字符串，并返回一个新的指针
		  yylval.text=strdup(yytext);
		  return(LITERALSTR);
		  } 


  /* define pragma here */
[#][[:space:]]*pragma[[:space:]]+list[[:space:]]+on   {opt_list=1;LIST;}
[#][[:space:]]*pragma[[:space:]]+list[[:space:]]+off  {opt_list=0;}

 /*
    yylval.text = strdup, strndup ... (yytext)
    yylval.dval = atoi, atof, strtod, strtol ... (yytext)
 */

[ \t\f\r]    LIST;

\n {
  LIST;
  LIST_FLUSH;
  line_no++, col_no = 1;
}

.  { LIST; fprintf(stderr, "[ERROR] line %4d:%3d lexical analyzer error %s\n", line_no, col_no - yyleng, yytext); }
