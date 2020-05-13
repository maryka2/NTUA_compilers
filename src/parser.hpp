/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
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
    T_and = 258,
    T_array = 259,
    T_begin = 260,
    T_boolean = 261,
    T_char = 262,
    T_dispose = 263,
    T_div = 264,
    T_do = 265,
    T_else = 266,
    T_end = 267,
    T_false = 268,
    T_forward = 269,
    T_function = 270,
    T_goto = 271,
    T_if = 272,
    T_integer = 273,
    T_label = 274,
    T_mod = 275,
    T_new = 276,
    T_nil = 277,
    T_not = 278,
    T_of = 279,
    T_or = 280,
    T_procedure = 281,
    T_program = 282,
    T_real = 283,
    T_result = 284,
    T_return = 285,
    T_then = 286,
    T_true = 287,
    T_var = 288,
    T_while = 289,
    T_eq = 290,
    T_gt = 291,
    T_lt = 292,
    T_ne = 293,
    T_ge = 294,
    T_le = 295,
    T_plus = 296,
    T_minus = 297,
    T_times = 298,
    T_rdiv = 299,
    T_dref = 300,
    T_ref = 301,
    T_assign = 302,
    T_semicolon = 303,
    T_dot = 304,
    T_lparen = 305,
    T_rparen = 306,
    T_colon = 307,
    T_comma = 308,
    T_lbracket = 309,
    T_rbracket = 310,
    T_intconst = 311,
    T_realconst = 312,
    T_charconst = 313,
    T_stringconst = 314,
    T_id = 315
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 75 "parser.ypp" /* yacc.c:1909  */

	int i;
	double f;
	char c;
	char *str;
	bool b;
	Type type;
	Expr *expr;
	Lvalue *lvalue;
	Rvalue *rvalue;
	Call *call;
	Header *header;
	Formal *formal;
	Block *block;
	Stmt *stmt;
	Id_vector *var_name_type_list;
	Formal_vector *formal_list;
	Expr_vector *expr_list;
	UnOp *unop;
	BinOp *binop;
	char op[3];
	Id *id;
	Local *local;

#line 140 "parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
