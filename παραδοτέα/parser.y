%{
#include <cstdio>
#include <stdlib.h>
%}

%token T_and "and"
%token T_array "array"
%token T_begin "begin"
%token T_boolean "boolean"
%token T_char "char"
%token T_dispose "dispose"
%token T_div "div"
%token T_do "do"
%token T_else "else"
%token T_end "end"
%token T_false "false"
%token T_forward "forward"
%token T_function "function"
%token T_goto "goto"
%token T_if "if"
%token T_integer "integer"
%token T_label "label"
%token T_mod "mod"
%token T_new "new"
%token T_nil "nil"
%token T_not "not"
%token T_of "of"
%token T_or "or"
%token T_procedure "procedure"
%token T_program "program"
%token T_real "real"
%token T_result "result"
%token T_return "return"
%token T_then "then"
%token T_true "true"
%token T_var "var"
%token T_while "while"
%token T_eq "="
%token T_gt ">"
%token T_lt "<"
%token T_ne "<>"
%token T_ge ">="
%token T_le "<="
%token T_plus "+"
%token T_minus "-"
%token T_times "*"
%token T_rdiv "/"
%token T_dref "^"
%token T_ref "@"
%token T_assign ":="
%token T_semicolon ";"
%token T_dot "."
%token T_rparen ")"
%token T_colon ":"
%token T_comma ","
%token T_lbracket "["
%token T_rbracket "]"

//%precedence T_dref	//pif
//%precedence T_not	//pif
%nonassoc T_eq T_gt T_lt T_le T_ge T_ne
%left T_plus T_minus T_or
%left T_time T_rdiv T_div T_mod T_and

/* %expect 1; */

%union{
	int i;
	double f;
	char str[80];
	bool b;
	Expr *expr;
	Lvalue *lvalue;
	Rvalue *rvalue;
	Call *call;
	Type type;
}

%token<str> T_id
%token<i> T_intconst
%token<f> T_realconst
%token<str> T_charconst
%token<str> T_stringconst

%type<expr>  expr
%type<lvalue>  lvalue
%type<rvalue>  rvalue
%type<call>  call		//pif
%type<str> var_name_list
%type<str> var_name_type_list
%type<type> type

%%

program				: "program" T_id ";" body "."
				;

body				: local_list block
				| block
				;

local_list			: local_list local
				| local
				;

local 				: "var" var_name_type_list
				| "label" var_name_list ";"
				| header ";" body ";"
				| "forward" header ";"
				;



var_name_type_list		: var_name_list ":" type ";" var_name_type_list	{ $$ = $1 +":"+ $3 +";"+ $5; }
				| var_name_list ":" type ";"			{ $$ = $1 +":"+ $3 +";"; }
				;


var_name_list			: T_id "," var_name_list			{ $$ = $1 +","+ $3; }
				| T_id						{ $$ = $1; }
				;

header				: "procedure" T_id "(" formal_list ")"
				| "function" T_id "(" formal_list ")" ":" type
				| "procedure" T_id "(" ")"
				| "function" T_id "(" ")" ":" type
				;


formal_list			: formal ";" formal_list
				| formal
				;

formal				: "var" var_name_list ":" type
				| var_name_list ":" type
				;

type				: "integer"				{ $$ = "integer"; }
				| "real"				{ $$ = "real"; }
				| "boolean"				{ $$ = "boolean"; }
				| "char"				{ $$ = "char"; }
				| "array" "[" T_intconst "]" "of" type	{ $$ = "array [" + $3 +"] of " + $6; }
				| "array" "of" type			{ $$ = "array of " +$3; }
				| "^" type				{ $$ = "^" + $2; }
				;

block				: "begin" stmt_list "end"
				| "begin" "end"
				;


stmt_list			: stmt ";" stmt_list
				| stmt_list ";"
				| ";" stmt_list		
				| stmt
				;


stmt				: lvalue ":=" expr
				| block
				| call
				| "if" expr "then" stmt "else" stmt
				| "if" expr "then" stmt 
				| "while" expr "do" stmt
				| T_id ":" stmt
				| "goto" T_id
				| "return"
				| "new" lvalue
				| "new" "[" expr "]" lvalue
				| "dispose" lvalue
				| "dispose" "[" "]" lvalue
				;



lvalue				: T_id			{ $$ = $1; }
				| "result"		{ $$ = "result"; }
				| T_stringconst		{ $$ = $1; }
				| lvalue "[" expr "]"
				| expr "^"		{ $$ = *$1; }
				| "(" lvalue ")"	{ $$ = $2; }
				;

rvalue				: T_intconst		{ $$ = $1; }
				| "true"		{ $$ = true; }
				| "false"		{ $$ = false; }
				| T_realconst		{ $$ = $1; }
				| T_charconst		{ $$ = $1; }
				| "(" rvalue ")"	{ $$ = $2; }
				| "nil"			{ $$ = nil; }
				| call			{ $$ = $1; }
				| "@" lvalue		{ $$ = &$2; }
				| "not" expr		{ $$ = ! $2; }
				| expr "+" expr		{ $$ = $1 + $3; }
				| expr "-" expr		{ $$ = $1 - $3; }
				| expr "*" expr		{ $$ = $1 * $3; }
				| expr "/" expr		{ $$ = 1.0 * $1 / $3; }
				| expr "div" expr	{ $$ = $1 / $3; }
				| expr "mod" expr	{ $$ = $1 % $3; }
				| expr "or" expr	{ $$ = $1 || $3; }
				| expr "and" expr	{ $$ = $1 && $3; }
				| expr "=" expr		{ $$ = $1 == $3; }
				| expr "<>" expr	{ $$ = $1 != $3; }
				| expr "<" expr		{ $$ = $1 < $3; }
				| expr "<=" expr	{ $$ = $1 <= $3; }
				| expr ">" expr		{ $$ = $1 > $3; }
				| expr ">=" expr	{ $$ = $1 >= $3; }
				| "+" expr		{ $$ = $2; }
				| "-" expr		{ $$ = (-1)*$2; }
				;

expr				: lvalue		{ $$ = $1; }
				| rvalue		{ $$ = $1; }
				;


call				: T_id "(" ")"
				| T_id "(" expr_list ")"
				;

expr_list			: expr
				| expr "," expr_list
				;

%%

int main ()
{
return yyparse();
}
