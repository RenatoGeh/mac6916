/* problogic.h */

#ifndef _PROBLOGIC_H
#define _PROBLOGIC_H

#include "expression.h"

/* variable data structure */
typedef struct {
  char* name; /* user-friendly name */
  int size;   /* domain size */
} variable;

/* model data structure */
typedef struct {
  int n;          /* the size of the vocabulary */
  int size;       /* size of the event space */
  variable **vars; /* list of (pointers) variables */
  double *p;      /* the distribution of valuations; 
		     valuations are ordered in lexicographically order from right to left */
} model;

/* valuation */
typedef struct {
  int n; /* vocabulary size (dimension) */
  variable **vars; /* vocabulary */
  int *v; /* valuation function */
} valuation;

int mypow(int n);

/* Allocates space for a model over vocabulary x1, ..., xn */
model *createBooleanModel(int n);

/* Allocates space for a model over given vocabulary */
model *createModel(int n, variable**);

/* increments state represented by 0/1-valued vector w of size n */
void next(int w[], int n);

/* creates a valuation over given vocabulary */
valuation *createValuation(int, variable**);

/* increment Valuation (in lexicographic order form right to left) */
void incValuation(valuation*);

/* prints out verbose information on model */
void display(model *m);

/* Computes probability of formula */
double query(formula* f, model *m);

#endif



