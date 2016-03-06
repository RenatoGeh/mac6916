#ifndef BAYES_H_
#define BAYES_H_

#include <initializer_list>
#include <vector>
#include <string>

#include "cpt.h"

/** Bayesian Network
 *
 */
class Bayes {
  public:
    Bayes(std::vector<Cpt> cpts);
    ~Bayes(void);

  private:
    std::vector<Cpt> cpt;
    
};

#endif
