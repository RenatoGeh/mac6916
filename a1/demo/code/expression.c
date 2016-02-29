#include "expression.h"
#include <stdio.h>
#include <stdlib.h>

/* creates a variable (leaf) node and returns pointer to it 

   @param var_id a strictly positive integer
   @return node representing variable
*/
formula* addVariable(int var_id) {
  formula *p;
  if ((p = malloc(sizeof(formula))) == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(1);
  }
  p->type = VARIABLE;
  p->var.id = var_id;
  return p;
}

/* creates an assignment (leaf) node and returns pointer to it 

   @param var_id a strictly positive integer specifying the id of a variable
   @param value a strictly positive integer specifying the value assignment
   @return node representing variable
*/
formula* addAssignment(int var_id, int value) {
  formula *p;
  if ((p = malloc(sizeof(formula))) == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(1);
  }
  p->type = ASSIGNMENT;
  p->eq.id = var_id;
  p->eq.value = value;
  return p;
}

/* creates a binary operation node and returns pointer to it 

   @param operator the type of operation to be performed (opEnum)
   @param op1 pointer to the left operand
   @param op2 pointer to the right operand
   @return node representing the operation
*/
formula* addOperation(int operator, formula* op1, formula* op2) {
  formula *p;
  if ((p = malloc(sizeof(formula))) == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(1);
  }
  p->type = OPERATOR;
  p->op.type = operator;
  p->op.leftOperand = op1;
  p->op.rightOperand = op2;
  return p;
}

/* creates a unary operation (not) and returns pointer to it 

   @param op operand
   @return node representing operation
*/
formula* addNot(formula* op) {
  formula *p;
  if ((p = malloc(sizeof(formula))) == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(1);
  }
  p->type = NOT;
  p->not.operand = op;
  return p;  
}


/* prints out formula 

   @param f the root node of the expression tree
*/
void printFormula(formula *f) {
  if (f == NULL) return;
  switch (f->type) {
  case VARIABLE:
    printf("X%d", f->var.id);
    return;
  case ASSIGNMENT:
    printf("X%d=%d", f->eq.id, f->eq.value);
    return;
  case NOT:
    printf("not "); printFormula(f->not.operand); 
    return;
  case OPERATOR:
    switch (f->op.type) {
    case AND:
      printf("("); printFormula(f->op.leftOperand); printf(" and "); printFormula(f->op.rightOperand); printf(")");
      return;
    case OR:
      printf("("); printFormula(f->op.leftOperand); printf(" or "); printFormula(f->op.rightOperand); printf(")");
      return;
    }
  }  
}

/* evaluates formula for given valuation v of size n 

   @param f the root node of an expression tree
   @param v integer vector representing valuation
   @returns truth-value of the expression at w
*/
int evaluate(formula *f, int v[]) {
  if (f == NULL) return -1;
  switch (f->type) {
  case VARIABLE:
    return v[f->var.id];
  case ASSIGNMENT:
    return v[f->eq.id]==f->eq.value;
  case NOT:
    return !evaluate(f->not.operand, v);
  case OPERATOR:
    switch (f->op.type) {
    case AND:
      return (evaluate(f->op.leftOperand, v) && evaluate(f->op.rightOperand, v));
    case OR:
      return ((evaluate(f->op.leftOperand, v)) || (evaluate(f->op.rightOperand, v)));
    }
  default:
    return -1;
  }
}

/* inserts formula in front of linked list

   @param f the content formula
   @return new front of the list
*/
ll_formula* push(ll_formula* r, formula *f) {
  ll_formula *c;
  if ((c = malloc(sizeof(ll_formula))) == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(1);
  }
  c->content = f;
  c->next = r;
  return c;
}

