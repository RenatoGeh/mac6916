#include "nbayes.h"

#include <cstdio>
#include <cstring>

#include <vector>
#include <deque>
#include <set>

#include <dai/varset.h>
#include <dai/factorgraph.h>

NaiveBayes::NaiveBayes(std::vector<dai::Factor> nodes) : c_(nodes.front()), atts_(nodes.begin()+1,
    nodes.end()), graph_(nodes), strides_(new size_t[nodes.size()-1]) {

  size_t ns = graph_.nrFactors();
  for (size_t i=0;i<ns;++i)
    joint_ *= graph_.factor(i);
  joint_.normalize();

  /* n : number of attributes. */
  int n = nodes.size()-1;
  strides_[n-1] = 1;
  for (int i=n-1;i>0;--i)
    /* Stride[i-1] = Stride[i]*|dom(Scope[i])| */
    strides_[i-1] = strides_[i]*((joint_.vars().begin()+i)->states());
}

NaiveBayes::~NaiveBayes(void) {
  delete[] strides_;
}

double NaiveBayes::Marginal(dai::Factor& jprob, std::deque<int>& val) {
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

size_t NaiveBayes::Classify(std::vector<dai::Var> atts, std::vector<int> val, bool output) {
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

std::vector<dai::Var> NaiveBayes::GetVars(std::vector<int> labels) {
  std::sort(labels.begin(), labels.end());
  std::vector<dai::Var> vars;
  for (auto it=labels.begin();it!=labels.end();++it)
    vars.push_back(graph_.var(*it));
  return vars;
}

namespace {
  /* Must be delete[]d when done. */
  int* get_data_fields(const std::string& line, int n) {
    int *store = new int[n];
    char *cpy = new char[line.size()+1];
    strcpy(cpy, line.c_str());
    char *tok = strtok(cpy, ", ");
    for (int i=0;i<n;++i) {
      store[i] = atoi(tok);
      tok = strtok(NULL, ", ");
    }
    delete[] cpy;
    return store;
  }
}

NaiveBayes NaiveBayes::Learn(const char *name, int n) {
  std::ifstream file;
  file.open(name);

  if (!file.is_open()) {
    fprintf(stderr, "Filename [%s]:\n", name);
    throw std::runtime_error("  Exception: failed to open file!\n");
  }

  /* N_joint is a vector of counting tables, where its first dimension equals to the state index of
   *  the class node. The second dimension refers to the j-th state of the attribute. Each element
   *  of N_joint is a counting table of an i-th attribute node. Each element of the table is the
   *  number of instances that are consistent with the indeces seen during learning.
   *
   * Example:
   *
   * @attribute A_1 {0,1,2} // na_1 = 3
   * @attribute A_2 {0,1}   // na_2 = 2
   *
   * @class C {0, 1}        // nc   = 2
   *
   * N_joints <- vector<int, int[][]>{<na_1, int[0..(na_1-1)][0..(nc-1)]>,
   *                                  <na_2, int[0..(na_2-1)][0..(nc-1)]>}
   */
  std::vector<std::pair<int, int**>> N_joint;
  int *N_c = nullptr, n_c = 0;
  std::string line;

  /* Discards the first line (@relation). */
  std::getline(file, line);
  /* Discards second line (empty line). */
  std::getline(file, line);

  for (int i=0;i<n+1;++i) {
    std::getline(file, line);

    /* Assumes a long yet finite number of states. */
    char states[102];
    /* Assumes that nominal values (e.g. {0,1,2}) contain no spaces and are in increasing order. */
    sscanf(line.c_str(), "%*s %*s {%s}", states);
    /* Counts the n number of states of attribute i. There are always n-1 commas. */
    int n_st=0;
    for (int j=0;states[j]!='\0';++j)
      if (states[j] == ',')
        ++n_st;
    ++n_st;
    /* Pushes the number of states in i and a counting table. */
    if (i<n)
      N_joint.push_back(std::make_pair(n_st, new int*[n_st]));
    else {
      N_c = new int[n_st];
      n_c = n_st;
      for (int j=0;j<n_c;++j)
        N_c[j] = 0;
    }
  }

  /* Finds a @data line. No spaces allowed before @. */
  while (std::getline(file, line))
    if (!line.compare(0, 5, "@data"))
      break;

  if (std::cin.bad() || std::cin.eof()) {
    fprintf(stderr, "IO/EOF error with file [%s]:\n", name);
    throw std::runtime_error("  Exception: failed to read. No @data field.");
  }

  /* Initializes to zero. */
  for (auto it=N_joint.begin();it!=N_joint.end();++it)  {
    int n_M = it->first;
    int **M = it->second;
    for (int i=0;i<n_M;++i) {
      M[i] = new int[n_c];
      for (int j=0;j<n_c;++j)
        M[i][j] = 0;
    }
  }

  /* Counts. */
  while (std::getline(file, line)) {
    int *values = get_data_fields(line, n+1), k=0;
    ++N_c[values[n]];
    for (auto it=N_joint.begin();it!=N_joint.end();++it,++k) {
      int **M = it->second;
      ++M[values[k]][values[n]];
    }
    delete[] values;
  }
  int total_N_c = 0;
  for (int i=0;i<n_c;++i)
    total_N_c += N_c[i];

  /* Constructs Factors through MLEs. */
  std::vector<dai::Factor> nodes;
  dai::Var c_var(0, n_c);
  dai::Factor c_node(c_var);
  for (int i=0;i<n_c;++i)
    /* MLE on a single var: (N[v=x]+delta_i)/(N+delta_t). */
    c_node.set(i, ((double)N_c[i]+(1./n_c))/((double)total_N_c+n_c));
  nodes.push_back(c_node);

  int __i=1;
  for (auto it=N_joint.begin();it!=N_joint.end();++it,++__i) {
    int n_M = it->first;
    dai::Var att_var(__i, n_M);
    /* Factor over att_var given c_var. */
    dai::Factor phi(dai::VarSet(att_var, c_var));
    int **M = it->second, _c = 0;
    for (int j=0;j<n_M;++j)
      for (int k=0;k<n_c;++k) {
        /* MLE on X->Y: (N[X=x,Y=y]+delta_i)/(N[Y=y]+delta_t). */
        phi.set(_c++, (((double)M[j][k])+(1./n_M))/((double)N_c[k]+n_M));
      }
    nodes.push_back(phi);
  }

  for (auto it=N_joint.begin();it!=N_joint.end();++it) {
    int n_M = it->first;
    int **M = it->second;
    for (int i=0;i<n_M;++i)
      delete[] M[i];
    delete[] M;
  }
  delete[] N_c;

  file.close();

  return NaiveBayes(nodes);
}
