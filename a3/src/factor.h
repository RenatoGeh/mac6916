#ifndef FACTOR_H_
#define FACTOR_H_

#include <utility>

#include "variable.h"
#include "utils.h"

/** A Factor (or table) representing instantiations and probabilities.
 *
 * Variable cells is a table of the following form:
 *
 * +------------------------------------------+
 * |X_0 X_1 X_2 ... X_n |  Pr(x_0,x_1,...,x_n)|
 * |------------------------------------------|
 * | 0   0   0  ...  0  |  Pr(0,  0  ,..., 0 )|
 * | .               .  |            .        |
 * | .   .           .  |            .        |
 * | .       .       .  |            .        |
 * |p_0 p_1 p_2 ... p_n |  Pr(p_0,p_1,...,p_n)|
 * +------------------------------------------+
 *
 * That is, each line is a pair composed of an array of instantiations and an associated
 * probability; and p_i is the maximum number of values variable X_i may take.
 *
 * For each pair:
 *   1. first  = Array<std::string> -> instantiations
 *   2. second = double             -> probability
 */
class Factor {
  public:
    /* Considers scope is sorted lexicographically and cells follows scope's order. */
    Factor(utils::Array<Variable> scope,
        utils::Array<std::pair<utils::Array<std::string>, double>> cells);
    ~Factor(void);

    const utils::Array<Variable>& Scope(void) const { return scope_; }

    /* Finds a probability with instanciation e.
     *
     * Since every element in the table has a different changing interval, we can find any
     * probability by summing all the variances plus their offsets.
     *
     * Let v_i be the variance of variable X_i and g_i the gap of that same variable. Then
     * we can find the position of each variable X_1,...,X_n given their instanciation
     * x_1,...,x_n with the following equation:
     *
     * SUM_{i=0}^s g_i * v_i = index
     *
     * where SUM is a summation and s is the number of variables in the table's scope. The
     * resulting integer index is the position of the desired probability.
     *
     * Pr(X_1=x_1,...,X_n=x_n) = cells_[index].second;
     *
     */
    double Find(utils::Array<std::string> e);

    friend Factor operator+(const Factor& lval, Variable rval) {
      return Factor::SumCopy(lval, rval);
    }

    /*friend Factor operator*(const Factor& lval, const Factor& rval) {
      return Factor::MultiplyCopy(lval, rval);
    }*/

    //Factor& operator+=(Variable rval) { return Factor::Sum(*this, rval); }
    //Factor& operator*=(const Factor& rval) { return Factor::Multiply(*this, rval); }

  public:
    static Factor SumCopy(const Factor& lval, Variable rval);
    //static Factor MultiplyCopy(const Factor& lval, const Factor& rval);

    //static Factor& Sum(Factor& lval, Variable rval);
    //static Factor& Multiply(Factor& lval, const Factor& rval);
  private:
    /* Variances */
    utils::Array<int> v_;
    /* Number of possible values in each variable. */
    utils::Array<int> sizes_;
    utils::Array<Variable> scope_;
    utils::Array<std::pair<utils::Array<std::string>, double>> cells_;
};

#endif
