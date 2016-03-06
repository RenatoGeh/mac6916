#include <cstdio>

#include <initializer_list>
#include <vector>
#include <string>

#include "row.h"
#include "cpt.h"
#include "variable.h"

#define ASIA 0
#define TUB 1
#define SMOKE 2
#define LUNG 3
#define EITHER 4
#define BRONC 5
#define XRAY 6
#define DYSP 7

int main(int argc, char *args[]) {
  Variable vars[] = {
    Variable("Asia", {"yes", "no"}), Variable("Tub", {"yes", "no"}),
    Variable("Smoke", {"yes", "no"}), Variable("Lung", {"yes", "no"}),
    Variable("Bronc", {"yes", "no"}), Variable("Either", {"yes", "no"}),
    Variable("X-ray", {"yes", "no"}), Variable("Dysp", {"yes", "no"})
  };

  Cpt tables[] = {
    Cpt(vars[ASIA], {}, {
          Row({"yes"}, 0.01),
          Row({"no"}, 0.99)
        }),
    Cpt(vars[TUB], {vars[ASIA]}, {
          Row({"yes", "yes"}, 0.05),
          Row({"no", "yes"}, 0.95),
          Row({"yes", "no"}, 0.01),
          Row({"no", "no"}, 0.99)
        }),
    Cpt(vars[SMOKE], {}, {
          Row({"yes"}, 0.5),
          Row({"no"}, 0.5)
        }),
    Cpt(vars[LUNG], {vars[SMOKE]}, {
          Row({"yes", "yes"}, 0.1),
          Row({"no", "yes"}, 0.9),
          Row({"yes", "no"}, 0.01),
          Row({"no", "no"}, 0.99)
        }),
    Cpt(vars[EITHER], {vars[TUB], vars[LUNG]}, {
          Row({"yes", "yes", "yes"}, 1),
          Row({"no", "yes", "yes"}, 0),
          Row({"yes", "yes", "no"}, 1),
          Row({"no", "yes", "no"}, 0),
          Row({"yes", "no", "yes"}, 1),
          Row({"no", "no", "yes"}, 0),
          Row({"yes", "no", "no"}, 0),
          Row({"no", "no", "no"}, 1)
        }),
    Cpt(vars[BRONC], {vars[SMOKE]}, {
          Row({"yes", "yes"}, 0.6),
          Row({"no", "yes"}, 0.4),
          Row({"yes", "no"}, 0.3),
          Row({"no", "no"}, 0.7)
        }),
    Cpt(vars[XRAY], {vars[EITHER]}, {
          Row({"yes", "yes"}, 0.98),
          Row({"no", "yes"}, 0.02),
          Row({"yes", "no"}, 0.05),
          Row({"no", "no"}, 0.95)
        }),
    Cpt(vars[DYSP], {vars[EITHER], vars[BRONC]}, {
          Row({"yes", "yes", "yes"}, 0.9),
          Row({"no", "yes", "yes"}, 0.1),
          Row({"yes", "yes", "no"}, 0.7),
          Row({"no", "yes", "no"}, 0.3),
          Row({"yes", "no", "yes"}, 0.8),
          Row({"no", "no", "yes"}, 0.2),
          Row({"yes", "no", "no"}, 0.1),
          Row({"no", "no", "no"}, 0.9)
        })
  };

  return 0;
}
