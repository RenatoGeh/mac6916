#include "bayes.h"

#include "factor.h"
#include "utils.h"
#include "variable.h"

Bayes::Node::Node(Variable var, Factor& cpt) : var_(var), cpt_(cpt) {

}

Bayes::Node::~Node(void) {}

Bayes::Bayes(utils::Array<std::pair<Variable, Factor&>> nodes) : adj_(nodes.Size()) {
  int s = nodes.Size();
  for (int i=0;i<s;++i)
    adj_[i] = new Bayes::Node(nodes[i].first, nodes[i].second);
  utils::Sort(adj_);

  for (int i=0;i<s;++i) {
    const utils::Array<Variable>& scp = adj_[i]->Cpt().Scope();
  }
}
