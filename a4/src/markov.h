#ifndef MARKOV_H_
#define MARKOV_H_

#include <vector>

#include "valuation.h"
#include "variable.h"
#include "factor.h"
#include "utils.h"

/* Markov Network */
class Markov {
  public:
    class Node {
      public:
        Node(Variable var);
        ~Node(void);

        const Variable& Var(void) const { return var_; }

        Node* Next(void) const { return next_; }
        Node* Prev(void) const { return prev_; }
        void Next(Node* next) { next_ = next; }
        void Prev(Node* prev) { prev_ = prev; }

        void Add(Factor f) { phi_.push_back(f); }

        const std::vector<Factor>& Phi(void) const { return phi_; }
      private:
        Variable var_;
        std::vector<Factor> phi_;

        Node* next_;
        Node* prev_;
    };

  public:
    Markov(utils::Array<Variable> scope, utils::Array<Factor> cliques);
    ~Markov(void);

    /* Inference: Posterior
     *
     * Computes posterior inference of the form P(X=query|E=evidence).
     * To compute this probability:
     *
     * 1. For each pair (u, v) in evidence, where u is a variable and v is u's variable consistent
     *    with valuation evidence, do:
     *    1.1 Restrict all factors whose scopes contain u consistently with v.
     * 2. For each variable Y in query, do:
     *    2.1 Find all factors that contain Y in their scope.
     *    2.2 Multiply all these factors and assign the result to a factor Q.
     * 3. Compute the partition function Z.
     * 4. Return Q[query]/Z.
     *
     */
    double Posterior(std::string query, Valuation evidence);

    /* Inference: Evidence
     *
     * Computes probability of evidence of the form P(E=e).
     * To compute this probability:
     *
     * 1. Multiply all factors and assign to factor Q.
     * 2. Compute the partition function Z.
     * 3. Sum out all variables not in E from factor Q.
     * 4. Return Q[e]/Z.
     *
     */
    double Evidence(Valuation e);

    /* Computes the partition function.
     *
     * 1. Multiply all factors and assign to factor Q.
     * 2. Sum out all variables.
     * 3. Return the resulting trivial factor.
     *
     */
    double Partition(void);
  private:
    utils::Array<Variable> scope_;
    utils::Array<Factor> factors_;
    utils::Array<Node*> adj_list_;
};

#endif
