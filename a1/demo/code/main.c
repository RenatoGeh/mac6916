/* Reads models and queries from file in format

  model
  var NAME1/SIZE1 .
  ...
  var NAMEN/SIZEN .
  dist value1 value2 ... valueM
  query EXPRESSION .  
*/

#define YY_NO_UNPUT
#include <stdio.h>
#include <stdlib.h>
#include "expression.h"
#include "problogic.h"

// prototype of bison-generated parser function
int yyparse();

int main(int argc, char **argv)
{
  if ((argc > 1) && (freopen(argv[1], "r", stdin) == NULL))
  {
    fprintf(stderr, "%s: File %s cannot be opened.\n", argv[0], argv[1]);
    exit( 1 );
  }

  extern ll_formula* yydata; /* linked list of formulas */
  extern model* yymodel; /* model */
  yyparse();
  display(yymodel);
  ll_formula *it = yydata; /* iterator starting at head */
  while (it) {
    printf("Prob{");
    printFormula(it->content);
    printf("} = %.3f\n", query(it->content, yymodel));
    it = it->next;
  }
  
  return 0;
}
