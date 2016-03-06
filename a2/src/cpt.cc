#include "cpt.h"

#include <initializer_list>
#include <vector>

#include "variable.h"
#include "row.h"

Cpt::Cpt(Variable self, std::vector<Variable> parents, std::vector<Row> rows) :
  rows_(rows) {
  scope_.push_back(self);
  for (auto var : parents)
    scope_.push_back(var);

  for (auto it=rows_.begin();it!=rows_.end();++it)
    it->Init(scope_);
}

Cpt::Cpt(Variable self, std::initializer_list<Variable> parents, std::initializer_list<Row> rows) :
  rows_(rows) {
  scope_.push_back(self);
  for (auto var : parents)
    scope_.push_back(var);

  for (auto it=rows_.begin();it!=rows_.end();++it)
    it->Init(scope_);
}

Cpt::~Cpt(void) {}

double Cpt::Pr(std::initializer_list<std::string> vals) {
  return Row::Find(vals, rows_);
}

double Cpt::Pr(std::vector<std::string> vals) {
  return Row::Find(vals, rows_);
}
