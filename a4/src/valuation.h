#ifndef VALUATION_H_
#define VALUATION_H_

#include <string>

#include "variable.h"
#include "utils.h"

class Valuation {
  public:
    Valuation(utils::Array<Variable> scope, utils::Array<std::string> vals);
    ~Valuation(void);

    const utils::Array<std::pair<Variable, std::string>>& Inst(void) { return inst_; }

  private:
    utils::Array<std::pair<Variable, std::string>> inst_;
};

#endif
