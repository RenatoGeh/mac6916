#ifndef NBAYES_H_
#define NBAYES_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <deque>

#include <dai/factorgraph.h>

class NaiveBayes {
  public:
    NaiveBayes(std::vector<dai::Factor> nodes);
    ~NaiveBayes(void);

    /* Inference. */

    /* Returns the marginal probability of a certain valuation.
     *
     *  Marginal(p, val) = Pr(X_0=val[0],X_1=val[1],...,X_{n-1}=val[n-1])
     *
     * Where p is the already summed out joint probability and val is the valuation for each
     * variable. The joint probability must be given to prevent recalculation.
     *
     * The returning value is a double in [0,1] that corresponds to the marginal probability of
     * a given valuation val.
     */
    double Marginal(dai::Factor& jprob, std::deque<int>& val);

    /* Gets the most probable class given attributes (MAP).
     *
     *  Classify(A, val) = argmax_c Pr(C=c|A=val)
     *
     * By default, output is set to true. If set to false, Classify will not ouput any messages.
     *
     * Returns a size_t indicating the state of C that maximizes the posterior probability.
     */
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

    /* Learns a Naive Bayes from a file that contains n attributes. */
    static NaiveBayes Learn(const char *name, int n);

  private:
    /* Class node. */
    dai::Factor c_;
    /* Attribute nodes. */
    std::vector<dai::Factor> atts_;

    /* Factor graph. */
    dai::FactorGraph graph_;

    /* Full joint distribution. */
    dai::Factor joint_;

    /* Strides of each variable. */
    size_t *strides_;
};

#endif
