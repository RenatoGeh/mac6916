#include "variable.h"

#include <string>
#include <vector>
#include <initializer_list>

Variable::Variable(std::string name, std::initializer_list<std::string> vals) : name_(name) {
  for (auto val : vals)
    vals_.push_back(val);
}

Variable::~Variable(void) {}
