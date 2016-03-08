#include "factor.h"

#include <vector>
#include <string>
#include <algorithm>
#include <utility>

#include "variable.h"

Factor::Factor(std::vector<Variable> scope,
    std::vector<std::vector<std::pair<std::string, std::string>>> vals,
    std::vector<double> pr) : scope_(scope), pr_(pr) {
  for (auto it=vals.begin();it!=vals.end();++it)
    vals_.push_back(std::vector<std::pair<std::string, std::string>>(*it));
}

Factor::~Factor(void) {}

Factor Factor::Multiply(const Factor& phi_1, const Factor& phi_2) {

}

Factor Factor::Sum(const Factor& phi, Variable var) {
  std::vector<Variable> scope;
  std::vector<std::vector<std::pair<std::string, std::string>>> vals;

  auto phi_scope = phi.Scope();
  auto phi_vals = phi.Values();
  auto phi_pr = phi.Probs();

  for (auto it=phi_scope.begin();it!=phi_scope.end();++it)
    if (it->Name().compare(var.Name()))
      scope.push_back(*it);

  /* All vectors must be of same size. */
  int size = phi_pr.size();
  /* Number of possible values var may take. */
  int p_vals = var.NumVals();
  /* Number of variables in phi. */
  int n_vars = phi_scope.size();

  int lines = phi_vals.size();
  std::vector<double> pr(lines, 0);
  bool *check = new bool[lines];

  for (int i=0;i<lines;++i)
    check[i] = false;

  for (int i=0;i<p_vals;++i) {
    for (int j=0;j<n_vars;++j) {
      auto pair_j = phi_vals.begin()+j;
      if (!pair_j->first.compare(var.Name())) {

      }
    }
  }

  return Factor(scope, vals, pr);
}
