#ifndef DISTRIBUTION_H_
#define DISTRIBUTION_H_

#include <initializer_list>
#include <string>

/** Joint (discrete) distribution.
 * name_   - name of the distribution
 * n_vars  - number of variables
 * n_vals  - number of possible values
 * n_probs - number of probabilities (n_vals^n_vars)
 * jprobs  - joint probabilities
 *
 * The size, |jprobs|, of probabilities is n_vals^n_vars=|jprobs|.
 * jprobs can be seen as a matrix (vars)x(joint probs).
 */
class JointDist {
  public:
    JointDist(std::string name, int n_vars, int n_vals, std::initializer_list<double> jprobs);
    ~JointDist(void);

  private:
    /* Name of distribution. */
    std::string name_;
    /* Number of variables. */
    int n_vars_;
    /* Number of possible values (must be discrete). */
    int n_vals_;
    /* Number of total probabilities (n_vals^n_vars). */
    int n_probs_;
    /* Table of joint probabilities. */
    double **jprobs_;
};

#endif
