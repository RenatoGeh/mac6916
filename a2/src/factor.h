#ifndef FACTOR_H_
#define FACTOR_H_

#include <vector>
#include <string>
#include <utility>

#include "variable.h"

/* A factor representing a joint distribution. */
class Factor {
  public:
    Factor(std::vector<Variable> scope,
        std::vector<std::vector<std::pair<std::string, std::string>>> vals,
        std::vector<double> pr);
    ~Factor(void);

    const std::vector<Variable>& Scope(void) const { return scope_; }
    const std::vector<std::vector<std::pair<std::string, std::string>>>& Values(void) const {
      return vals_;
    }
    const std::vector<double>& Probs(void) const { return pr_; }

    /* Multiplies two factors and returns a new factor whose scope i Scope(phi_1) union
     * Scope(phi_2).
     *
     * Idea:
     *
     * S = Intersection(phi_1.scope, phi_2.scope)
     * probs[|phi_1.scope|+|phi_2.scope|] = {0,...,0}
     *
     * For every l line in max(phi_1, phi_2):
     *   If (all values S in phi_1) == (all values S in phi_2):
     *     For each v variable in phi_1:
     *
     *
     *
     */
    static Factor Multiply(const Factor& phi_1, const Factor& phi_2);
    /* Sums out variable var from factor phi.
     *
     * Note: assumes var is in phi's scope.
     *
     * Idea:
     *
     * # |var.vals| = number of possible values of var
     * probs[|phi.scope|-|var.vals|] = {0,...,0}
     *
     * For every p possible value of var:
     *   For every l line in phi:
     *     If l.val == p:
     *       probs[l.index] += l.pr
     *
     * return new Factor(phi.scope-var, phi.vals-var, pr)
     */
    static Factor Sum(const Factor& phi, Variable var);
  private:
    std::vector<Variable> scope_;
    /* Instantiations in the format <Var_name, Var_val> (e.g. <Dysp, yes>). */
    std::vector<std::vector<std::pair<std::string, std::string>>> vals_;
    std::vector<double> pr_;
};

#endif
