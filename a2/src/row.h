#ifndef ROW_H_
#define ROW_H_

#include <vector>
#include <initializer_list>
#include <string>
#include <algorithm>

#include "variable.h"

/* Row of a CPT
 *
 * In a CPT we may have X_1=x_1,...,X_n=x_n variables. For each row of a CPT we have a probability
 * of the form:
 *
 * Pr(X_1=x_1|X_2=x_2,...,X_n=x_n) = pr
 *
 * Thus we have an exponential number of probabilities. Each row is a probability and has variables
 * set to a unique instantiation in the CPT.
 *
 * Variable vals_ is the unique instantiation X_1=x_1,...,X_n=x_n.
 * Variable pr_ is the probability pr of the unique instantiation.
 */
class Row {
  public:
    Row(std::initializer_list<std::string> vals, double pr);
    ~Row(void);

    void Init(std::vector<Variable>& scope);

    const std::vector<std::string>& Values(void) const { return vals_; }
    const std::string& CompactValue(void) const { return compact_; }
    double Pr(void) const { return pr_; }

    /* Finds the probability of the given instantiation. That is, let vals={x_1,...,x_n}:
     *
     * Pr(x_1|x_2,...,x_n) = pr
     *
     * Returns the value of pr. If not found return < 0.
     *
     * Note: considers the first value is always the value of the query variable.
    */
    static double Find(std::initializer_list<std::string> vals, std::vector<Row>& cpt);
    static double Find(std::vector<std::string> vals, std::vector<Row>& cpt);

    /* Transforms variable values into a compact string.
     * Example:
     *   Variable names: Dysp, Bronc, Lung, Smoke
     *   Values:         yes , no   , yes , no
     *   Compact: Dysp=yes,Bronc=no,Lung=yes,Smoke=no
     *
     * It is guaranteed for the representation to be unique since all variable names must be
     * different and there cannot be more than one same variable in an instatiation.
     */
    static std::string Compact(std::initializer_list<std::string> inst);
    static std::string Compact(std::vector<std::string> inst);

    /* Compares if a compact representation is equal to an explicit var-val.
     * Example:
     *   var: Dysp; val: yes
     *   cval: Dysp=yes
     *   Returns true
     *
     *   var: Smoke; val: no
     *   cval: Bronc=no
     *   Returns false
     */
    static bool Equals(std::string cval, Variable var, std::string val);
  private:
    std::vector<std::string> vals_;
    std::string compact_;
    double pr_;
};

#endif
