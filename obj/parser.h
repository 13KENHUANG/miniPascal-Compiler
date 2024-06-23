/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_OBJ_PARSER_H_INCLUDED
# define YY_YY_OBJ_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PROGRAM = 258,
    VAR = 259,
    ARRAY = 260,
    OF = 261,
    INTEGER = 262,
    REAL = 263,
    STRING = 264,
    PROCEDURE = 265,
    PBEGIN = 266,
    END = 267,
    IF = 268,
    THEN = 269,
    ELSE = 270,
    WHILE = 271,
    DO = 272,
    NOT = 273,
    AND = 274,
    OR = 275,
    LPAREN = 276,
    RPAREN = 277,
    SEMICOLON = 278,
    DOT = 279,
    COMMA = 280,
    COLON = 281,
    LBRACE = 282,
    RBRACE = 283,
    DOTDOT = 284,
    ASSIGNMENT = 285,
    INTEGERNUM = 286,
    LITERALSTR = 287,
    REALNUMBER = 288,
    SCIENTIFIC = 289,
    FUNCTION = 290,
    IDENTIFIER = 291,
    ADDOP = 292,
    SUBOP = 293,
    MULOP = 294,
    DIVOP = 295,
    LTOP = 296,
    GTOP = 297,
    EQOP = 298,
    GETOP = 299,
    LETOP = 300,
    NEQOP = 301,
    T_CONST = 302,
    T_TYPE = 303,
    T_RECORD = 304,
    T_GOTO = 305,
    T_REPEAT = 306,
    T_UNTIL = 307,
    T_CASE = 308,
    T_FOR = 309,
    T_TO = 310,
    T_DOWNTO = 311,
    T_READ = 312,
    T_WRITE = 313,
    T_WRITELN = 314,
    T_UNEQUAL = 315,
    T_MOD = 316
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 56 "parser.y"

  int val;
  char* text;
  double dval;
  TreeNode node;
  LocType loc;

#line 127 "obj/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_OBJ_PARSER_H_INCLUDED  */
