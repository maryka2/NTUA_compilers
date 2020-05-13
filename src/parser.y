%{
#include <cstdio>
#include <stdlib.h>
#include "ast.hpp"
#include "lexer.hpp"
#define YYERROR_VERBOSE
SymbolTable st;
std::unordered_map<string, SymbolEntry*> globals;  // Global hash table
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
%token T_semicolon ";"
%token T_dot "."
%token T_rparen ")"
%token T_colon ":"
%token T_comma ","

%nonassoc<op> "=" ">" "<" ">=" "<=" "<>"
%left<op> "+" "-" "or"
%left<op> "*" "/" "div" "mod" "and"
%nonassoc<op> "not"
%nonassoc<op> "^"
%nonassoc<op> "@"
%nonassoc<op> "[" "]"

%expect 1;

%union{
	int i;
	double f;
	char c;
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
	Stmt *stmt;
	Id_vector *var_name_type_list;
	Formal_vector *formal_list;
	Expr_vector *expr_list;
	UnOp *unop;
	BinOp *binop;
	char op[3];
	Id *id;
	Local *local;
}

%token<i> T_intconst
%token<f> T_realconst
%token<c> T_charconst
%token<str> T_stringconst

%token<str> T_id
%type<expr> expr
%type<lvalue> lvalue
%type<rvalue> rvalue
%type<call> call
%type<header> header
%type<formal> formal
%type<block> block body local_list program stmt_list
%type<local> local
%type<stmt> stmt
%type<var_name_type_list> var_name_type_list var_name_list
%type<formal_list> formal_list
%type<expr_list> expr_list
%type<type> type


%%

program				: "program" T_id ";" body "."	{ $4->sem(); $$ = $4; }
				;

body				: local_list block	{ $1->merge($2); $$ = $1; }
				;

local_list			: /*nothing*/		{ $$ = new Block(); }
				| local_list local	{ $1->append_local($2); $$ = $1; }
				;

local 				: "var" var_name_type_list	{ $$ = new Local("var", $2); }
				| "label" var_name_list ";"	{ $$ = new Local("label", $2); }
				| header ";" body ";"		{ $$ = new Local_after_block($1, $3); }
				| "forward" header ";"		{ $$ = new Local($2); }
				;



var_name_type_list		: var_name_list ":" type ";"		{
  for (Id* id_ptr : $1->get_vector()) {
    id_ptr->set_type($3);
  }
  $$ = $1;
}
				| var_name_type_list var_name_list ":" type ";" {
  for (Id* id_ptr : $2->get_vector()) {
    id_ptr->set_type($4);
    $1->append_id(id_ptr);
  }
  $$ = $1;
}
				;

var_name_list			: var_name_list "," T_id 			{ $1->append_id(new Id(std::string($3))); $$ = $1; }
				| T_id						{ $$ = new Id_vector(new Id(std::string($1))); }
				;

header				: "procedure" T_id "(" formal_list ")"			{ $$ = new Header(std::string($2), $4); }
				| "function" T_id "("  formal_list ")" ":" type		{ $$ = new Header(std::string($2), $4, $7); }
				| "procedure" T_id "(" ")"				{ $$ = new Header(std::string($2)); }
				| "function" T_id "(" ")" ":" type			{ $$ = new Header(std::string($2), $6); }
				;

formal_list			: formal			{ $$ = new Formal_vector($1); }
				| formal_list ";" formal   	{ $1->append_formal($3); $$ = $1; }
				;

formal				: "var" var_name_list ":" type		{
  for (Id* id_ptr : $2->get_vector()) {
    id_ptr->set_type($4);
  }
  $$ = new Formal("var", $2, $4);
}
				| var_name_list ":" type		{
  for (Id* id_ptr : $1->get_vector()) {
    id_ptr->set_type($3);
  }
  $$ = new Formal($1, $3);
}
				;

type				: "integer"				{ $$ = type_integer(); }
				| "real"				{ $$ = type_real(); }
				| "boolean"				{ $$ = type_boolean(); }
				| "char"				{ $$ = type_char(); }
				| "array" "[" T_intconst "]" "of" type	{ $$ = type_arrayI($3, $6); }
				| "array" "of" type			{ $$ = type_arrayII($3); }
				| "^" type				{ $$ = type_pointer($2); }
				;

block				: "begin" stmt_list "end"		{ $$ = $2; }
				;


stmt_list			: /*nothing*/				{ $$ = new Block(); }
				| stmt_list stmt ";"			{ $1->append_stmt($2); $$ = $1; }
				;


stmt				: /*nothing*/				{ $$ = new EmptyStmt(); }
				| lvalue ":=" expr			{ $$ = new Assignment($1, $3); }
				| block					{ $$ = $1; }
				| call					{ $1->is_statement(); $$ = $1; }
				| "if" expr "then" stmt "else" stmt	{ $$ = new If($2, $4, $6); }
				| "if" expr "then" stmt			{ $$ = new If($2, $4); }
				| "while" expr "do" stmt		{ $$ = new While($2, $4); }
				| T_id ":" stmt				{ $$ = new Label(std::string($1), $3); }
				| "goto" T_id				{ $$ = new Goto(std::string($2)); }
				| "return"				{ $$ = new Return(); }
				| "new" lvalue				{ $$ = new New($2); }
				| "new" "[" expr "]" lvalue		{ $$ = new New($3, $5); }
				| "dispose" lvalue			{ $$ = new Dispose("pointer", $2); }
				| "dispose" "[" "]" lvalue		{ $$ = new Dispose("array_pointer", $4); }
				;



lvalue				: T_id			{ $$ = new Id(std::string($1)); }
				| "result"		{ $$ = new Id("result"); }
				| T_stringconst		{ $$ = new Stringconst(std::string($1)); }
				| lvalue "[" expr "]"	{ $$ = new Array($1, $3); }
				| expr "^"		{ $$ = new Dereference($1); }
				| "(" lvalue ")"	{ $$ = $2; }
				;


rvalue				: T_intconst		{ $$ = new Intconst($1); }
				| "true"		{ $$ = new Bool("true"); }
				| "false"		{ $$ = new Bool("false"); }
				| T_realconst		{ $$ = new Realconst($1); }
				| T_charconst		{ $$ = new Charconst($1); }
				| "(" rvalue ")"	{ $$ = $2; }
				| "nil"			{ $$ = new Nil(); }
				| call			{ $$ = $1; }
				| "@" expr		{ $$ = new UnOp($1, $2); }
			        | "not" expr   		{ $$ = new UnOp($1, $2); }
			        | "+" expr   		{ $$ = new UnOp($1, $2); }
			        | "-" expr   		{ $$ = new UnOp($1, $2); }
			        | expr "+" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "-" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "*" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "/" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "div" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "mod" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "or" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "and" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "=" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "<>" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "<" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr "<=" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr ">" expr  		{ $$ = new BinOp($1, $2, $3); }
			        | expr ">=" expr  		{ $$ = new BinOp($1, $2, $3); }
			        ;



expr				: lvalue		{ $$ = $1; }
				| rvalue		{ $$ = $1; }
				;


call				: T_id "(" ")"			{ $$ = new Call(std::string($1)); }
				| T_id "(" expr_list ")"	{ $$ = new Call(std::string($1), $3); }
				;

expr_list			: expr		{ $$ = new Expr_vector($1); }
				|  expr_list "," expr	{ $1->append_expr($3); $$ = $1; }
				;

%%

int main ()
{
  int result = yyparse();
  if (result == 0) {
    std::cout << "Success.\n";
  }
  return result;
}

