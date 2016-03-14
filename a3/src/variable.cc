#include "variable.h"

#include <string>
#include <algorithm>
#include <utility>
#include <initializer_list>

#include "utils.h"

Variable::Variable(std::string name, std::initializer_list<std::string> vals) :
  name_(name), vals_(vals) {
  utils::Sort(vals_);
}

Variable::~Variable(void) {}

int Variable::Index(std::string query) const {
  std::string* index = utils::BinarySearch(vals_, query);
  return index==vals_.End()?-1:index-vals_.Begin();
}
