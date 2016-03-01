#ifndef DISTRIBUTION_H_
#define DISTRIBUTION_H_

#include <initializer_list>
#include <string>

/** Joint distribution for Boolean variables.
 * name_   - name of the distribution
 * n_vars  - number of variables
 * n_probs - number of probabilities (2^n_vars)
 * jprobs  - joint probabilities
 *
 * The size, |jprobs|, of probabilities is 2^n_vars=|jprobs|.
 * jprobs can be seen as a vector of probabilities with each index i=(var_1...var_n)_2 a binary
 * representation of the values of all variables.
 *
 * The joint probabilities must be in increasing order. For instance:
 *   X_1  X_2  X_3  Pr(.)
 *    0    0    0   0.300
 *    0    0    1   0.075
 *    0    1    0   0.100
 *    0    1    1   0.025
 *    1    0    0   0.150
 *    1    0    1   0.225
 *    1    1    0   0.050
 *    1    1    1   0.075
 */
class JointDist {
  public:
    JointDist(std::string name, int n_vars, double *jprobs);
    JointDist(std::string name, int n_vars, std::initializer_list<double> jprobs);
    ~JointDist(void);

    /* Computes the prior probability Pr(X_var=val) by summing out all values in which X_var=val.
     * Note: function considers var>0. If var=0, then Pr(.)=1.
    */
    double Prior(int var, bool val);

    /* Computes the conditional probability Pr(X_query=val|X_evidence=e).
     * Note1: function considers query,evidence>0. If query=0 Pr(.|e); if evidence=0 Pr(val|.)=
     * =Pr(val); if both zero, then 1.
     * Note2: If Pr(.|e), it returns 1 and prints the new updated distribution to stdout.
     */
    double Cond(int query, bool val, int evidence, bool e);

    /* Prints self. */
    void Print(void);
  private:
    /* Name of distribution. */
    std::string name_;
    /* Number of variables. */
    int n_vars_;
    /* Number of possible values (must be discrete). */
    int n_probs_;
    /* Table of joint probabilities. */
    double *jprobs_;
    /* Size of jprobs_=2^n_vars_. */
    int size_;

    /* Number of true values until var turns false. Take the table above as an example. Let G(X_i)
     * be the gap of variable X_i. G(X_1)=4, G(X_2)=2, G(X_3)=1. G(X_i)=2^(n_vars_-i).
     */
    int Gap(int var);

    /* Computes Pr(.|X_evidence=e). */
    double* AllCond(int evidence, bool e);

    /* Prints the joint distribution probs of size |probs|=size. */
    static void PrintDist(std::string name, double *probs, int n_vars);
};

#endif
