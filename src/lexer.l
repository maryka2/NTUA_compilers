%{ #include <string.h>
#include "lexer.hpp"

#define T_eof 0
#define T_id 1
#define T_and 2
#define T_array 3
#define T_begin 4
#define T_boolean 5
#define T_char 6
#define T_dispose 7
#define T_div 8
#define T_do 9
#define T_else 10
#define T_end 11
#define T_false 12
#define T_forward 13
#define T_function 14
#define T_goto 15
#define T_if 16
#define T_integer 17
#define T_label 18
#define T_mod 19
#define T_new 20
#define T_nil 21
#define T_not 22
#define T_of 23
#define T_or 24
#define T_procedure 25
#define T_program 26
#define T_real 27
#define T_result 28
#define T_return 29
#define T_then 30
#define T_true 31
#define T_var 32
#define T_while 33
#define T_intconst 34
#define T_realconst 35
#define T_eq 36
#define T_gt 37
#define T_lt 38
#define T_ne 39
#define T_ge 40
#define T_le 41
#define T_plus 42
#define T_minus 43
#define T_times 44
#define T_rdiv 45
#define T_dref 46
#define T_ref 47
#define T_assign 48
#define T_semicolon 49
#define T_dot 50
#define T_lparen 51
#define T_rparen 52
#define T_colon 53
#define T_comma 54
#define T_lbracket 55
#define T_rbracket 56
#define T_charconst 57
#define T_stringconst 58

void ERROR (const char msg []);

int lineno =1;
%}

L [A-DF-Za-df-z]
E [Ee]
D [0-9]
W [ \t\n\r]

%x COMMENT
%option noyywrap
%%

<<EOF>> { return T_eof; }
"and" { return T_and; }
"array" { return T_array; }
"begin" { return T_begin; }
"boolean" { return T_boolean; }
"char" { return T_char; }
"dispose" { return T_dispose; }
"div" { return T_div; }
"do" { return T_do; }
"else" { return T_else; }
"end" { return T_end; }
"false" { return T_false; }
"forward" { return T_forward; }
"function" { return T_function; }
"goto" { return T_goto; }
"if" { return T_if; }
"integer" { return T_integer; }
"label" { return T_label; }
"mod" { return T_mod; }
"new" { return T_new; }
"nil" { return T_nil; }
"not" { return T_not; }
"of" { return T_of; }
"or" { return T_or; }
"procedure" { return T_procedure; }
"program" { return T_program; }
"real" { return T_real; }
"result" { return T_result; }
"return" { return T_return; }
"then" { return T_then; }
"true" { return T_true; }
"var" { return T_var; }
"while" { return T_while; }
"=" { return T_eq; }
">" { return T_gt; }
"<" { return T_lt; }
"<>" { return T_ne; }
">=" { return T_ge; }
"<=" { return T_le; }
"+" { return T_plus; }
"-" { return T_minus; }
"*" { return T_times; }
"/" { return T_rdiv; }
"^" { return T_dref; }
"@" { return T_ref; }
":=" { return T_assign; }
";" { return T_semicolon; }
"." { return T_dot; }
"(" { return T_lparen; }
")" { return T_rparen; }
":" { return T_colon; }
"," { return T_comma; }
"[" { return T_lbracket; }
"]" { return T_rbracket; }


({L}|{E})({L}|{E}|{D}|"_")* { return T_id; }
{D}+ { return T_intconst; }
({D}+("."{D}*({E}("+"|"-")?{D}+)?)?) { return T_realconst; }


("'"((("\\")?({L}|{E}|{D}|"\ "|"\!"|"\#"|"\$"|"\%"|"\&"|"\("|"\)"|"\*"|"\+"|"\,"|"\-"|"\."|"\/"|"\:"|"\;"|"\<"|"\="|"\>"|"\?"|"\@"|"\["|"\]"|"\^"|"\_"|"\`"|"\{"|"\|"|"\}"|"\~"))|("\\"("\\"|"'"|"\""))|(\t|\r))"'") { return T_charconst; }

"\""((("\\")?({L}|{E}|{D}|"\ "|"\!"|"\#"|"\$"|"\%"|"\&"|"\("|"\)"|"\*"|"\+"|"\,"|"\-"|"\."|"\/"|"\:"|"\;"|"\<"|"\="|"\>"|"\?"|"\@"|"\["|"\]"|"\^"|"\_"|"\`"|"\{"|"\|"|"\}"|"\~"))|("\\"("\\"|"'"|"\""))|(\t|\r))*"\"" { return T_stringconst; }


[ \t\r]+ { /* nothing */ }
\n { lineno++; }

"(*" { BEGIN (COMMENT); }
<COMMENT>"*)" { BEGIN (INITIAL); }
<COMMENT>\n { lineno++; }
<COMMENT>"*" { /* nothing */ }
<COMMENT>[^*\n]+ { /* nothing */ }

. { ERROR("illegal token"); }

%%

void yyerror(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

void ERROR (const char msg [])
{
	fprintf (stderr, "ERROR, line %d: %s\n", lineno, msg);
	exit(1);
}
