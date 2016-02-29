#include "jointdist.h"

#include <cmath>

#include <initializer_list>
#include <vector>
#include <string>

JointDist::JointDist(std::string name, int n_vars, int n_vals,
    std::initializer_list<double> jprobs) : name_(name), n_vars_(n_vars), n_vals_(n_vals){
  n_probs_ = pow(n_vals_, n_vars_);
  jprobs_ = new double*[n_probs_];
  for (int i=0;i<n_probs_;++i) {
    jprobs_[i] = new double[n_vars+1];
    for (int j=0;j<n_vars;++j)
  }
}

JointDist::~JointDist(void) {}
