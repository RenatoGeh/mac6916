#include "markov.h"

#include <vector>

#include "variable.h"
#include "factor.h"
#include "utils.h"

Markov::Node::Node(Variable var) : var_(var), next_(nullptr), prev_(nullptr) {}
Markov::Node::~Node(void) {}

Markov::Markov(utils::Array<Variable> scope, utils::Array<Factor> cliques) :
  scope_(scope), factors_(cliques), adj_list_(cliques.Size()) {
  int cliques_s = cliques.Size();
  int scope_s = scope.Size();

  for (int i=0;i<scope_s;++i)
    adj_list_[i] = new Node(scope[i]);

  for (int i=0;i<cliques_s;++i) {
    const utils::Array<Variable>& local_scope = cliques[i].Scope();
    for (int j=0;j<local_scope.Size();++j) {
      int index = BinarySearch(scope, local_scope[j])-local_scope.Begin();
      adj_list_[index]->Add(cliques[i]);
    }
  }
}

double Markov::Posterior(std::string query, Valuation evidence) {

}

double Markov::Evidence(Valuation e) {

}
