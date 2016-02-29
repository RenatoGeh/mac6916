/* Encodes the model in Table 3.3, page 35 of Darwiche's book and computes probability queries. */

#include <stdio.h>
#include <stdlib.h>
#include "expression.h"
#include "problogic.h"

int main(int argc, char **argv)
{
  int i;
  /* Model in Table 3.3, page 35 of Darwiche 2009's book */
  double p[] = {.576, .144, .064, .016, .008, .032, .032, .128};

  model *alarm = createBooleanModel(3);
  for (i=0; i < alarm->size; i++) alarm->p[i] = p[i]; /* alarm->N is the model dimension (8) */
  display(alarm);  
    
  formula *var1 = addVariable(0);
  formula *var2 = addVariable(1);
  formula *var3 = addVariable(2);
  formula *not = addNot(var3);
  formula *and = addOperation(AND, var2, not);
  formula *or = addOperation(OR, var1, and);
  //  marginals 
  formula **f;  formula *marginals[] = {var1, var2, var3, NULL};
  for (f = marginals; *f != NULL; f += 1) {
    printf("Prob{"); printFormula(*f); printf("} = %.4f\t", query(*f, alarm));
    printf("Prob{not "); printFormula(*f); printf("} = %.4f\n", query(addNot(*f), alarm));
  }
  // a more complex query
  printf("Prob{"); printFormula(or); printf("} = %.4f\n", query(or, alarm));
  return 0;
}
