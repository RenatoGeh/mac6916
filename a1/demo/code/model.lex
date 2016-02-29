/* FLEX TOKENIZER  */
/* model.lex */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tok.h"
int yyerror(char *s);
%}
%option noyywrap
%option nounput
%option noinput
%%

"model"   { return YYMODEL; }
"var"     { return YYVAR; }
"dist"    { return YYDIST; }
"query"   { return YYQUERY; }
"not"     { return YYNOT; }
"and"     { return YYAND; }
"or"      { return YYOR; }
[0-9]+	  { yylval.value = atoi(yytext); return YYINT; }
([0-9])?"."[0-9]+ { yylval.real = atof(yytext); return YYREAL; }
[a-zA-Z0-9]+ { yylval.index = strdup(yytext); return YYSTRING; }
[&|()=./] { return *yytext; }
[ \t\n]*  {}  /* skip blanks */
#[^\n]*\n {}  /* skip comments */
.	  { yyerror("Unknown character"); }

%%

