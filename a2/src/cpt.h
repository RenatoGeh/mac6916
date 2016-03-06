#ifndef CPT_H_
#define CPT_H_

#include <initializer_list>
#include <vector>

#include "variable.h"
#include "row.h"

/** Conditional Probability Table
 *
 * CPT wrt variable self given parents.
 * List rows are the probabilities given unique variable instantiations.
 *
 * The CPT is ordered lexicographically with the first variable being the query variable.
 *
 */
class Cpt {
  public:
    /* Pr(self|parents) = pr. */
    Cpt(Variable self, std::vector<Variable> parents, std::vector<Row> rows);
    Cpt(Variable self, std::initializer_list<Variable> parents, std::initializer_list<Row> rows);
    ~Cpt(void);

    const std::vector<Variable>& Scope(void) const { return scope_; }
    const std::vector<Row>& Parameters(void) const { return rows_; }

    double Pr(std::initializer_list<std::string> vals);
    double Pr(std::vector<std::string> vals);

  private:
    /* CPT Scope.
     * First element is always node variable. Rest are parents.
     */
    std::vector<Variable> scope_;
    /* Valarray containing probabilities. */
    std::vector<Row> rows_;
};

#endif
