#include "jointdist.h"

#include <cmath>
#include <cstdio>
#include <cstring>

#include <initializer_list>
#include <vector>
#include <string>
#include <algorithm>

JointDist::JointDist(std::string name, int n_vars, double *jprobs) : name_(name), n_vars_(n_vars) {
  size_ = pow(2, n_vars_);
  jprobs_ = new double[size_];
  for (int i=0;i<size_;++i)
    jprobs_[i] = jprobs[i];
}

JointDist::JointDist(std::string name, int n_vars, std::initializer_list<double> jprobs) :
  name_(name), n_vars_(n_vars) {
  size_ = pow(2, n_vars_);
  jprobs_ = new double[size_];
  int i=0;
  for (double p : jprobs) {
    jprobs_[i] = p;
    ++i;
  }
}

JointDist::~JointDist(void) {
  delete[] jprobs_;
}

int JointDist::Gap(int var) {
  return pow(2, n_vars_-var);
}

double JointDist::Prior(int var, bool val) {
  if (!var) return 1;

  int gap = Gap(var);
  double p = 0;

  /* Since the distribution is ordered lexicographically, then 0 is always first. If val is false
   * then it starts at the beggining (0). If it is true it must begin after the gap.
   */
  int init = val?gap:0;
  for (int i=init;i<size_;i+=gap)
    for (int j=0;j<gap;++j)
      p += jprobs_[i+j];

  return p;
}

double JointDist::Cond(int query, bool val, int evidence, bool e) {
  if (!query && !evidence) return 1;
  if (!evidence) return Prior(query, val);

  if (!query) {
    double *cond_probs = AllCond(evidence, e);

    JointDist::PrintDist(name_ + " conditioned.", cond_probs, n_vars_);

    delete[] cond_probs;

    return 1;
  }

  int qgap = Gap(query), egap = Gap(evidence);
  int qinit = val?qgap:0, einit = e?egap:0;
  int max, min;

  if (qgap > egap)
    max = qgap, min = egap;
  else
    max = egap, min = qgap;

  double p = 0;

  for (int i=qinit+einit;i<size_;i+=max)
    for (int j=0;j<max;j+=min)
      p += jprobs_[i+j];

  return p;
}

double* JointDist::AllCond(int evidence, bool e) {
  double *cond_probs = new double[size_];
  double p_e = Prior(evidence, e);
  int gap = Gap(evidence);
  int init = e?gap:0;

  memset(cond_probs, 0, size_*sizeof(double));

  for (int i=init;i<size_;i+=gap)
    for (int j=0;j<gap;++j)
      cond_probs[i+j] = jprobs_[i+j]/p_e;

  return cond_probs;
}

void JointDist::Print(void) {
  JointDist::PrintDist(name_, jprobs_, n_vars_);
}

void JointDist::PrintDist(std::string name, double *probs, int n_vars) {
  int size = pow(2, n_vars);
  name.insert(0, "Joint distribution ");
  puts(name.c_str());
  for (int i=0;i<n_vars;++i)
    printf(" X_%d ", i);
  puts("  P(.)");
  for (int i=0;i<size;++i) {
    int p2 = (int) pow(2, n_vars-1);
    for (int j=n_vars;j>0;--j) {
      printf("  %d  ", (i/p2)%2);
      p2 /= 2;
    }
    printf("  %.3f\n", probs[i]);
  }
}
