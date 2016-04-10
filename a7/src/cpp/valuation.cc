#include "valuation.h"

#include <string>

#include "utils.h"
#include "variable.h"

Valuation::Valuation(utils::Array<Variable> scope, utils::Array<std::string> vals) :
  inst_(scope.Size()), scope_(scope), vals_(vals) {
  int s = scope.Size();
  for (int i=0;i<s;++i)
    inst_[i] = std::make_pair(scope[i], vals[i]);
}

Valuation::~Valuation(void) {}

