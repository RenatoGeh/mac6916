#include "factor.h"

#include <string>
#include <utility>

#include "utils.h"

Factor::Factor(utils::Array<Variable> scope,
    utils::Array<std::pair<utils::Array<std::string>, double>> cells) :
  v_(scope.Size()), sizes_(scope.Size()), scope_(scope), cells_(cells) {

  int s = scope.Size();
  v_[s-1] = 1;
  for (int i=0;i<s;++i)
    sizes_[i] = scope_[i].Size();
  for (int i=s-2;i>=0;--i)
    v_[i] = v_[i+1] * sizes_[i+1];
}

Factor::~Factor(void) {}

double Factor::Find(utils::Array<std::string> e) {
  int index = 0;

  for (int i=0;i<scope_.Size();++i)
    index += scope_[i].Index(e[i])*v_[i];

  return cells_[index].second;
}

Factor Factor::SumCopy(const Factor& lval, Variable rval) {
  utils::Array<Variable> new_scope(lval.scope_.Size()-1);

  /* Copies scope minus rval. */
  int mark_elim_var=-1;
  for (int i,j=i=0;i<new_scope.Size();++i) {
    Variable& var = lval.scope_[i];
    if (rval.Name().compare(var.Name()))
      new_scope[j++] = Variable(var);
    else
      mark_elim_var = i;
  }

  /* New number of lines is the original number of lines divided by the possible values of rval.
   * We guarantee old_lines is divisible by rval.Size() because the total number of lines is the
   * product of the power of all possible values of each variable to their respective indices.
   */
  int old_lines = lval.cells_.Size();
  int new_lines = old_lines/rval.Size();
  utils::Array<std::pair<utils::Array<std::string>, double>> new_cells(new_lines);

  for (int i=0;i<new_lines;++i)
    new_cells[i] = std::make_pair(utils::Array<std::string>(new_scope.Size()), 0.0);

  int var_index = utils::BinarySearch(lval.scope_, rval)-lval.scope_.Begin();
  int v = lval.v_[var_index];
  int upper_bound = old_lines - v;
  for (int i,j=i=0;i<upper_bound;++i,++j) {
    if (i % v) i += v;
    /* This loop iterates through all other instances of rval. */
    double p = 0;
    for (int k=0;k<rval.Size();++k)
      p += lval.cells_[v*k].second;
    new_cells[j].second = p;
    /* This loop copies all variable instances minus rval's to the new table. */
    int cell_size = lval.cells_[i].first.Size();
    utils::Array<std::string>& vals_ref = lval.cells_[i].first;
    for (int k,l=k=0;k<cell_size;++k)
      if (k == mark_elim_var) continue;
      else new_cells[j].first[l++] = vals_ref[k];
  }

  return Factor(new_scope, new_cells);
}

Factor Factor::RestrictCopy(const Factor& lval, Valuation rval) {
  utils::Array<std::pair<Variable, std::string>>& inst = rval.Inst();
  utils::Array<int> indeces(inst.Size());

  int sc_size = scope.Size();
  int k = 0;
  int new_table_size = 1;

  for (int i=0;i<sc_size;++i)
    if (scope[i] == inst[k].first)
      indeces[k++] = i, new_table_size *= inst[k].first.Size();

  int table_size = cells_.Size();
  int ind_size = indeces.Size();

  k = 0;
  utils::Array<std::pair<utils::Array<std::string>, double>> n_cells(new_table_size);

  for (int i=0;i<table_size;++i) {
    std::pair<utils::Array<std::string>, double>& line = cells[i];
    bool matches = true;
    for (int j=0;j<ind_size;++j)
      if (line.first[indeces[j]] != inst[j].second)
        matches = false;
    if (matches) {
      utils::Array<std::string> new_line;
      int old_line_size = line.first.Size();
      n_cells[k++] = std::make_pair(
    }
  }
}

/* TODO. */
/*Factor Factor::MultiplyCopy(const Factor& lval, const Factor& rval) {
  utils::Array<Variable> new_scope(utils::Union(lval.scope_, rval.scope_));
}*/
