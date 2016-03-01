#include <cstdio>
#include <cmath>

#define str(s) #s
#define PRINT(exp, pr) printf("Pr(" str(exp) ") = %.3f", pr)
#define PRINTLN(exp, pr) printf("Pr(" str(exp) ") = %.3f\n", pr)

#include "jointdist.h"

int main(int argc, char *args[]) {
  int n;
  scanf("%d ", &n);
  double *in = new double[n];
  int pw = pow(2, n);
  for (int i=0;i<pw;++i)
    scanf("%lf ", &in[i]);

  JointDist jdist("Ex.1", n, in);

  puts("Prior probabilities:");

  PRINTLN(A=true, jdist.Prior(1, true));
  PRINTLN(B=true, jdist.Prior(2, true));
  PRINTLN(C=true, jdist.Prior(3, true));

  puts("\nPosterior probabilities:");

  //PRINTLN(.|C=true, jdist.Cond(0, false, 3, true));
  PRINTLN(A=true|C=true, jdist.Cond(1, true, 3, true));
  PRINTLN(B=true|C=true, jdist.Cond(2, true, 3, true));

  return 0;
}
