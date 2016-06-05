#ifndef AODC_H_
#define AODC_H_

#include <cstdio>

#include <iostream>
#include <fstream>

#include <vector>
#include <deque>

#include <dai/factorgraph.h>

/* Average One-Dependence Classifier. */
class AODC {
  AODC(std::vector<dai::Factor> nodes);
  ~AODC(void);

  /* Inference. */

  double Marginal(dai::Factor& jprob, std::deque<int>& val);

  size_t Classify(std::vector<dai::Var> atts, std::vector<int> val, bool output=true);

  std::vector<dai::Var> GetVars(std::vector<int> labels);

  /* Input/Output. */

  void WriteToFile(const char *name) {
    fprintf(stdout, "Writing graph to file [%s]...\n", name);
    graph_.WriteToFile(name);
  }

  void DrawGraph(const char *name) {
    std::filebuf file;
    file.open(name, std::ios::out);
    std::ostream out(&file);
    fprintf(stdout, "Writing graph to dot file [%s]...\n", name);
    graph_.printDot(out);
    file.close();
  }

  /* Learning. */
  static AODC Learn(const char *name, int n);

  private:
    /* Class node. */
    dai::Factor c_;
    /* Chosen dependency node. */
    dai::Factor d_;
    /* Attribute nodes. */
    std::vector<dai::Factor> atts_;

    /* Factor graph. */
    dai::FactorGraph graph_;

    /* Full joint distribution. */
    dai::Factor joint_;
};

#endif
