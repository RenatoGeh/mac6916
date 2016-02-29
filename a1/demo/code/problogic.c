/* Implements probabilistic models based on propositional logic */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "problogic.h"


//inline
int mypow(int n) {
  return 1 << n;
}

/* Allocates space for a model over boolean variables x1, ..., xn 

   @param n model dimension
   @return pointer to model created
*/
model *createBooleanModel(int n) {
  int i;
  model *m = malloc(sizeof(model));
  if (m == NULL) { fprintf(stderr, "ERROR: Could not allocate model\n"); exit(1); }
  m->n = n; /*  size of vocabulary */
  m->size = mypow(n); /* number of valuations (model size) */
  m->vars = malloc(n*sizeof(variable*));
  for (i=0; i < n; i++) {
    variable *v = malloc(sizeof(variable));
    if (v == NULL) { fprintf(stderr, "ERROR: Could not allocate model\n"); exit(1); }
    v->name = malloc((i+2)*sizeof(char)); /* waste of memory! */
    sprintf(v->name, "X%d", i);
    v->size = 2;
    m->vars[i] = v;
  }
  m->p = malloc(m->size*sizeof(double)); /* probability function */
  if (m->p == NULL) { fprintf(stderr, "ERROR: Could not allocate model\n"); exit(1); }
  return m;
}

/* Allocates space for a model over given variables 

   @param n number of variables
   @param vars list of variables (array of pointers to variables)
   @return pointer to model created
*/
model *createModel(int n, variable **vars) {
  int i;
  model *m = malloc(sizeof(model));
  if (m == NULL) { fprintf(stderr, "ERROR: Could not allocate model\n"); exit(1); }
  m->n = n; /*  size of vocabulary */
  m->size = 1; /* number of valuations (model size) */
  m->vars = vars;
  for (i=0; i < n; i++) {
    m->size *= m->vars[i]->size;
  }
  m->p = malloc(m->size*sizeof(double)); /* probability function */
  if (m->p == NULL) { fprintf(stderr, "ERROR: Could not allocate model\n"); exit(1); }
  return m;
}

/* increments state represented by 0/1-valued vector w of size n */
void next(int w[], int n) {
  int i;
  for (i = n-1; i >= 0 && w[i] == 1; i--)
    w[i] = 0;
  if (i >= 0) w[i] = 1;
}

/* creates a valuation over given vocabulary 

   @param n vocabulary size
   @param vars vocabulary (pointer to array of pointers to vars)
   @return first valuation in lexicographic order
*/
valuation *createValuation(int n, variable **vars) {
  valuation *v = malloc(sizeof(valuation));
  int *vv = calloc(n, sizeof(int));
  if (v == NULL || vv == NULL) { fprintf(stderr, "ERROR: Could not allocate valuation\n"); exit(1); }
  v->n = n;
  v->vars = vars;
  v->v = vv;
  return v;
}

/* increments valuation */
void incValuation(valuation *v) {
  int i;
  for (i = v->n-1; i >= 0 && v->v[i] == v->vars[i]->size-1; i--)
    v->v[i] = 0;
  if (i >= 0) v->v[i] = v->vars[i]->size-1;
}

/* prints out verbose information on model */
void display(model *m) {
  int i, j;
  double acc = 0.0;
  valuation *v = createValuation(m->n, m->vars);  
  printf("dimension: %d \t size: %d\n", m->n, m->size);
  printf("variables:");
  for (i = 0; i < m->n; i++)
    printf(" [X%d]%s/%d", i, m->vars[i]->name, m->vars[i]->size); 
  printf("\n");
  printf("Prob.    Valuation\n");
  for (i = 0; i < m->size; i++) {
    printf("%.6f ", m->p[i]);
    acc += m->p[i]; /* probability mass */
    for (j = 0; j < m->n; j++)
      printf("%d ", v->v[j]);
    printf("\n");
    incValuation(v);
  }
  printf("total mass: %.2f\n", acc);
}


/* Computes probability of formula 

   @param f the expression tree of a Boolean formula
   @param m a probabilistic model
   @return the probability of f according to m
*/
double query(formula* f, model *m) {
  int i;
  valuation *v = createValuation(m->n, m->vars);  /* valuation */  
  double p = 0.0;
  for (i=0; i < m->size; i++, incValuation(v)) if (evaluate(f, v->v)) p += m->p[i];
  return p;
}


