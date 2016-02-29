/* expression.h */
/* Boolean expressions are represented as binary trees */

#ifndef _EXPRESSION_H
#define _EXPRESSION_H

typedef enum { VARIABLE, ASSIGNMENT, NOT, OPERATOR } nodeTypeEnum;
typedef enum { AND, OR} opEnum;  

/* proposition node */
typedef struct {
  int id;
} variableFormula;

/* assignment node */
typedef struct {
  int id;
  int value;
} assignmentFormula;

/* binary operator node */
typedef struct {
  opEnum type;    /* operation */
  struct st_formula *leftOperand;
  struct st_formula *rightOperand;  
} operatorFormula;

/* unary operator node */
typedef struct {
  struct st_formula *operand;
} notFormula;

/* non-speciailized formula node */
struct st_formula {
  nodeTypeEnum type;       /* node type */
  union {
    variableFormula var;      /* proposition (leaf) node */
    assignmentFormula eq;    /* assignment (leaf) node */
    operatorFormula op;       /* binary operator node */
    notFormula      not;      /* unary operator node */
  };
};
typedef struct st_formula formula;

/* cell of linked list of formulas */
struct st_formula_cell {
  formula *content;
  struct st_formula_cell *next;
};
typedef struct st_formula_cell ll_formula;


void printFormula(formula*); /* prints out formula */
int evaluate(formula*, int v[]); /* evaluates a formula tree according to a valuation*/
formula* addVariable(int); /* add variable node */
formula* addAssignment(int, int); /* add assignment node */
formula* addOperation(int, formula*, formula*); /* add binary operation node */
formula* addNot(formula*); /* add unary operation node */

ll_formula *push(ll_formula*, formula*); /* inserts formula in front of list */

#endif 
