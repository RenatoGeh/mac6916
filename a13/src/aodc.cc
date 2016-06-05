#include "aodc.h"

#include <cstdio>
#include <cstring>

#include <deque>
#include <vector>

#include <dai/factorgraph.h>

AODC::AODC(std::vector<dai::Factor> nodes) : c_(nodes.front()), d_(*(nodes.begin()+1)),
  atts_(nodes.begin()+1, nodes.end()), graph_(nodes) {
  size_t ns = graph_.nrFactors();
  for (size_t i=0;i<ns;++i)
    joint_ *= graph_.factor(i);
  joint_.normalize();
}

AODC::~AODC(void) {}

double AODC::Marginal(dai::Factor& jprob, std::deque<int>& val) {
  /* Must construct a stride vector for each joint distribution. */
  int n = jprob.vars().size();
  size_t *strides = new size_t[n];
  strides[n-1] = 1;
  for (int j=n-1;j>0;--j) {
    //fprintf(stdout, "Just checking: %lu\n", strides[j]*((jprob.vars().begin()+j)->states()));
    strides[j-1] = strides[j]*((jprob.vars().begin()+j)->states());
  }

  int index, i=index=0;

  for (auto it=val.begin();it!=val.end();++it,++i)
    index += (*it)*strides[i];

  delete[] strides;
  return jprob[index];
}

namespace {
  bool set_pr_cmp(const std::pair<double, size_t> &l, const std::pair<double, size_t> &r) {
    if (l.first > r.first)
      return true;
    else if(l.first == r.first)
      return l.second < r.second;
    return false;
  }
}

size_t AODC::Classify(std::vector<dai::Var> atts, std::vector<int> val, bool output) {
  /* Finds marginals p(atts) and p(C, atts). */
  dai::VarSet den(atts.begin(), atts.end());
  dai::Var& c = c_.vars().front();
  atts.insert(atts.begin(), c);
  dai::VarSet num(atts.begin(), atts.end());
  dai::Factor marg(joint_.marginal(num)/joint_.marginal(den));

  /* Prepares valuations. */
  std::deque<int> vars;

  for (auto it=val.begin();it!=val.end();++it)
    vars.push_back(*it);

  /* Finds max. */
  double max = 0;
  size_t argmax = 0, ns = c.states();
  std::set<std::pair<double, size_t>, bool(*)(
      const std::pair<double, size_t>&, const std::pair<double, size_t>&)> p(set_pr_cmp);

  for (size_t i=0;i<ns;++i) {
    vars.push_front(i);
    double pr = Marginal(marg, vars);
    p.insert(std::make_pair(pr, i));
    if (pr > max) {
      max = pr;
      argmax = i;
    }
    vars.pop_front();
  }

  /* Prints probabilities. */
  if (output) {
    fprintf(stdout, "Most probable: C=%lu with Pr(C=%lu|A)=%.5f.\nWith other probabilities in "
        "decreasing order:\n", argmax, argmax, max);
    for (auto it=p.begin();it!=p.end();++it)
      fprintf(stdout, "C=%lu -> Pr(C=%lu|A)=%.5f\n", it->second, it->second, it->first);
  }

  return argmax;
}

std::vector<dai::Var> AODC::GetVars(std::vector<int> labels) {
  std::sort(labels.begin(), labels.end());
  std::vector<dai::Var> vars;
  for (auto it=labels.begin();it!=labels.end();++it)
    vars.push_back(graph_.var(*it));
  return vars;
}

AODC AODC::Learn(const char *name, int n) {
  std::ifstream file;
  file.open(name);

  if (!file.is_open()) {
    fprintf(stderr, "Filename [%s]:\n", name);
    throw std::runtime_error("  Exception: failed to open file!\n");
  }


}
