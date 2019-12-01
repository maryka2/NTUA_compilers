%{
#include <cstdio>
#include <stdlib.h>
#include "ast.hpp"
#include "lexer.hpp"

SymbolTable st;
std::vector<int> rt_stack;

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
%token T_ne "<>"
%token T_ge ">="
%token T_le "<="
%token T_assign ":="
%token T_semicolon ';'
%token T_dot '.'
%token T_rparen ')'
%token T_colon ':'
%token T_comma ','

%nonassoc<op> '=' '>' '<' ">=" "<=" "<>"
%left<op> '+' '-' "or" 
%left<op> '*' '/' "div" "mod" "and" 
%nonassoc<op> "not"
%nonassoc<op> '^'
%nonassoc<op> '@'
%nonassoc<op> '[' ']'

%expect 1;

%union{
	int i;
	double f;
	char str[80];
	bool b;
	Type type;
	Expr *expr;
	Lvalue *lvalue;
	Rvalue *rvalue;
	Call *call;
	Header *header;
	Formal *formal;
	Block *block;
	Stmt *stmt
	Var_ntl *var_name_type_list;
	Formal_l *formal_list;
	Expr_l *expr_list;
	Unop *unop;
	Binop *binop;
	char op[3];
}

%token<str> T_id
%token<i> T_intconst
%token<f> T_realconst
%token<str> T_charconst
%token<str> T_stringconst

%type<expr>  expr
%type<lvalue>  lvalue
%type<rvalue>  rvalue
%type<call>  call
%type<str> var_name_list
%type<header> header
%type<formal> formal
%type<block> block body local_list program stmt_list  local
%type<stmt> stmt
%type<var_name_type_list> var_name_type_list
%type<formal_list> formal_list
%type<expr_list> expr_list
%type<type> type


%%

program				: "program" T_id ';' body '.'	{ $$ = new Program($4); }
				;

body				: local_list block	{ $1->merge($2); $$ = $1; }
				;

local_list			: /*nothing*/		{ $$ = new Block(); }
				| local_list local	{ $1->append_local($2); $$ = $1; }
				;

local 				: "var" var_name_type_list	{ $$ = new Var($2); }
				| "label" var_name_list ';'	{ $$ = new Label($2); }
				| header ';' body ';'		{ $$ = new Header_Body($1, $3); }
				| "forward" header ';'		{ $$ = new Forward($2); }
				;



var_name_type_list		: var_name_list ':' type ';'		{ $$ = new Name_Type($1, $3); }
				| var_name_type_list var_name_list ':' type ';' { $1->append_name_type($2, $4); $$ = $1; }
				;


var_name_list			: var_name_list ',' T_id 			{ $1->append_name($3); $$ = $1; }
				| T_id						{ $$ = $1; }
				;

header				: "procedure" T_id '(' formal_list ')'			{ $$ = new Procedure($2, $4); }
				| "function" T_id '('  formal_list ')' ':' type		{ $$ = new Function($2, $4, $7); }
				| "procedure" T_id '(' ')'				{ $$ = new Procedure($2); }
				| "function" T_id '(' ')' ':' type			{ $$ = new Function($2, $6); }
				;


formal_list			: formal			{ $$ = $1; }
				| formal_list ';' formal   	{ $1->append_formal($3); $$ = $1; }
				;

formal				: "var" var_name_list ':' type		{ $$ = new By_Ref($2, $4); }
				| var_name_list ':' type		{ $$ = new By_Val($1, $3); }
				;

type				: "integer"				{ $$ = TYPE_integer; }
				| "real"				{ $$ = TYPE_real; }
				| "boolean"				{ $$ = TYPE_boolean; }
				| "char"				{ $$ = TYPE_char; }
				| "array" '[' T_intconst ']' "of" type	{ $$ = TYPE_arrayI; }
				| "array" "of" type			{ $$ = TYPE_arrayII; }
				| '^' type				{ $$ = TYPE_pointer; }
				;

block				: "begin" stmt_list "end"		{ $$ = $2; }
				;


stmt_list			: /*nothing*/				{ $$ = new Block(); }
				| stmt_list stmt ';'			{ $1->append_name_type($2); $$ = $1; }
				;


stmt				: /*nothing*/				{ $$ = new Block(); }
				| lvalue ":=" expr			{ $$ = new Assignment($1, $3); }
				| block					{ $$ = $1; }
				| call					{ $$ = $1; }
				| "if" expr "then" stmt "else" stmt	{ $$ = new If($2, $4, $6); }
				| "if" expr "then" stmt			{ $$ = new If($2, $4); }
				| "while" expr "do" stmt		{ $$ = new While($2, $4); }
				| T_id ':' stmt				{ $$ = new Label1($1, $3); }
				| "goto" T_id				{ $$ = new Goto($2); }
				| "return"				{ $$ = new Return(); }
				| "new" lvalue				{ $$ = new New($2); }
				| "new" '[' expr ']' lvalue		{ $$ = new New1($3, $5); }
				| "dispose" lvalue			{ $$ = new Dispose($2); }
				| "dispose" '[' ']' lvalue		{ $$ = new Dispose1($4); }
				;



lvalue				: T_id			{ $$ = new Id($1); }
				| "result"		{ $$ = new Result(); }
				| T_stringconst		{ $$ = new Stringconst($1); }
				| lvalue '[' expr ']'	{ $$ = new Array($1, $3); }
				| expr '^'		{ $$ = new Pointer($1); }
				| '(' lvalue ')'	{ $$ = $2; }
				;

rvalue				: T_intconst		{ $$ = new Intconst($1); }
				| "true"		{ $$ = new Bool("true"); }
				| "false"		{ $$ = new Bool("false"); }
				| T_realconst		{ $$ = new Realconst($1); }
				| T_charconst		{ $$ = new Charconst($1); }
				| '(' rvalue ')'	{ $$ = $2; }
				| "nil"			{ $$ = new Nil(); }
				| call			{ $$ = $1; }
				| '@' expr		{ $$ = new Op($1, $2); }
			        | "not" expr   		{ $$ = new UnOp($1, $2); }
			        | '+' expr   		{ $$ = new UnOp($1, $2); }
			        | '-' expr   		{ $$ = new UnOp($1, $2); }
			        | expr '+' expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr '-' expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr '*' expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr '/' expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "div" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "mod" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "or" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "and" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr '=' expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "<>" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr '<' expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "<=" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr '>' expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr ">=" expr  		{ $$ = new BinOp($1, $2, $3); }
			        ;




expr				: lvalue		{ $$ = $1; }
				| rvalue		{ $$ = $1; }
				;


call				: T_id '(' ')'			{ $$ = new Call($1); }
				| T_id '(' expr_list ')'	{ $$ = new Call($1, $3); }
				;

expr_list			: expr			{ $$ = $1; }
				|  expr_list ',' expr	{ $1->append_name_type($3); $$ = $1; }
				;

%%

int main ()
{
return yyparse();
}
