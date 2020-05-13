%option noyywrap

%{
#include <string.h>
#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int lineno =1;
%}

L [A-DF-Za-df-z]
E [Ee]
D [0-9]
W [ \t\n\r]

%x COMMENT
%%

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

({L}|{E})({L}|{E}|{D}|"_")* { yylval.str = strdup(yytext); return T_id; }
{D}+ { yylval.i = stoi(yytext); return T_intconst; }
({D}+("."{D}*({E}("+"|"-")?{D}+)?)?) { yylval.f = stod(yytext); return T_realconst; }


("'"((("\\")?({L}|{E}|{D}|"\ "|"\!"|"\#"|"\$"|"\%"|"\&"|"\("|"\)"|"\*"|"\+"|"\,"|"\-"|"\."|"\/"|"\:"|"\;"|"\<"|"\="|"\>"|"\?"|"\@"|"\["|"\]"|"\^"|"\_"|"\`"|"\{"|"\|"|"\}"|"\~"))|("\\"("\\"|"'"|"\""))|(\t|\r))"'") { yylval.c = yytext[0]; return T_charconst; }

"\""((("\\")?({L}|{E}|{D}|"\ "|"\!"|"\#"|"\$"|"\%"|"\&"|"\("|"\)"|"\*"|"\+"|"\,"|"\-"|"\."|"\/"|"\:"|"\;"|"\<"|"\="|"\>"|"\?"|"\@"|"\["|"\]"|"\^"|"\_"|"\`"|"\{"|"\|"|"\}"|"\~"))|("\\"("\\"|"'"|"\""))|(\t|\r))*"\"" { yylval.str = strdup(yytext); return T_stringconst; }


[ \t\r]+ { /* nothing */ }
\n { lineno++; }

"(*" { BEGIN (COMMENT); }
<COMMENT>"*)" { BEGIN (INITIAL); }
<COMMENT>\n { lineno++; }
<COMMENT>"*" { /* nothing */ }
<COMMENT>[^*\n]+ { /* nothing */ }

. { fprintf(stderr, "ERROR, line %d: illigal token\n", lineno); exit(1); }

%%

void yyerror(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}
