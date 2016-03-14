#ifndef BAYES_H_
#define BAYES_H_

#include <vector>

#include "factor.h"
#include "utils.h"
#include "variable.h"

/** A Bayesian Network */
class Bayes {
  public:
    class Node {
      public:
        Node(Variable var, Factor& cpt);
        ~Node(void);

        Factor& Cpt(void) const { return cpt_; }

        const utils::Array<Node*>& Ch(void) { return ch_; }
        const utils::Array<Node*>& Pa(void) { return pa_; }

        void Ch(utils::Array<Node*> ch) { ch_ = ch; }
        void Pa(utils::Array<Node*> pa) { pa_ = pa; }

        bool operator>(const Node& r) { return var_ > r.var_; }
        bool operator<(const Node& r) { return var_ < r.var_; }
        bool operator==(const Node& r) { return var_ == r.var_; }
        bool operator>=(const Node& r) { return var_ >= r.var_; }
        bool operator<=(const Node& r) { return var_ <= r.var_; }

      private:
        Variable var_;
        Factor& cpt_;
        utils::Array<Node*> ch_;
        utils::Array<Node*> pa_;
    };

  public:
    Bayes(utils::Array<std::pair<Variable, Factor&>> nodes);
    ~Bayes(void);

  private:
    /* Adjacency list representing the bayesian network's DAG. */
    utils::Array<std::vector<Node*>> adj_;
};

#endif
