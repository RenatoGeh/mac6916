/* Boolean Expression Parser */
/* To be used with bison */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expression.h"
#include "problogic.h"
#define  YYMAXVARS 100
 
  int yyerror(char *s);
  int yylex(void);
  int yynvars; /* total num of variables */
  variable *yyvars[YYMAXVARS]; /* variables */
  ll_formula* yydata; /* queries */
  int yygetvarid(char *name); /* variable id look-up */
  void yysetvarid(char *name, int id); /* assign id to variable */
  void yyshowVariables(); /* lists variables created */
  void yycreateVariable(char*,int); /* creates a variable */
  void yycreateModel(); /* creates model and store it in yymodel */
  model *yymodel; /* model */
  int yynreals;
  /* variable name look-up table: linked list implementation */
  struct yyvr_st {
    char *name;
    int id;
    struct yyvr_st *next;
  };
  typedef struct yyvr_st yyvrec;
  yyvrec *yyvlist;
%}

%union{
  int                value;
  char               *index;
  double             real;
  struct st_formula  *nptr;
}

%token <value> YYINT
%token <index> YYSTRING
%token <real> YYREAL
%token YYMODEL YYVAR YYDIST YYQUERY YYAND YYOR YYNOT

%left YYAND YYOR
%right YYNOT

%type <nptr> expression term factor

%%

program:  YYMODEL model query          
;

model:  variables YYDIST distarray '.' { }
;

distarray:
| distarray YYREAL       { yymodel->p[yynreals++]=$2; }
;

variables : variable { yycreateModel(); }
;

variable: 
| variable YYVAR YYSTRING '/' YYINT '.'  { yycreateVariable($3,$5); }            
;

query: 
| query YYQUERY expression '.'     { yydata = push(yydata, $3); }
;

expression: term
| expression YYOR term  { $$ = addOperation(OR, $1, $3); }
;

term: factor
| term YYAND factor      { $$ = addOperation(AND, $1, $3); }
;

factor:  YYNOT factor    { $$ = addNot($2); }
| '(' expression ')'     { $$ = $2; }          
| YYSTRING               { $$ = addVariable(yygetvarid($1)); }
| YYSTRING '=' YYINT     { $$ = addAssignment(yygetvarid($1), $3); }
;

%%

ll_formula *yydata = NULL;
int yynvars = 0;
int yynreals = 0;
yyvrec *yyvlist = NULL;

int yygetvarid(char *name) {
  yyvrec *var = yyvlist;
  for (var = yyvlist; var != NULL && strcmp(var->name,name); var = var->next);
  if (var != NULL) return var->id;
  printf("%s\n", name);
  yyerror("Variable not found!");
  return 0;
}

void yysetvarid(char *name, int id) {
    yyvrec *var = malloc(sizeof(yyvrec));
    var->name = name;
    var->id = id;
    var->next = yyvlist;
    yyvlist = var;
}

void yycreateModel() {
  int i;
  variable **vars = malloc(yynvars*sizeof(variable*));
  for (i = 0; i < yynvars; i++) vars[i]=yyvars[i];
  yymodel = createModel(yynvars, vars);
}

void yyshowVariables() {
  int i;
  printf("Variables read: %d.\n", yynvars);
  for (i = 0; i < yynvars; i++)
    printf("X%d:=%s/%d. ",  i, yyvars[i]->name, yyvars[i]->size);    
  printf("\n");
}

void yycreateVariable(char *name, int size) {
  if (yynvars == YYMAXVARS) yyerror("Too many variables!");
  variable *v = malloc(sizeof(variable));
  v->name = name;
  v->size = size;
  yyvars[yynvars] = v;
  yysetvarid(name, yynvars);
  printf("Renamed variable %s to X%d.\n", v->name, yynvars);
  yynvars++;
}

int yyerror(char* s)
{
  fprintf(stderr, "%s\n", s);
  exit(1);
}

