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
  const utils::Array<Variable>& scope = lval.Scope();
  const utils::Array<std::pair<Variable, std::string>>& inst = rval.Inst();
  utils::Array<int> indices(inst.Size());
  utils::Array<Variable> m_scope = utils::Minus(rval.Scope(), lval.Scope());
  utils::Array<int> m_indices(m_scope.Size());
  int sc_size = scope.Size();
  int k=0, l=0;
  int new_table_size = 1;

  /* Gets all indices to the scope array that match valuation rval. */
  for (int i=0;i<sc_size;++i) {
    if (scope[i] == inst[k].first)
      indices[k++] = i, new_table_size *= inst[k].first.Size();
    if (scope[i] == m_scope[l])
      m_indices[l++] = i;
  }

  int table_size = lval.cells_.Size();
  int ind_size = indices.Size();

  k = 0;
  utils::Array<std::pair<utils::Array<std::string>, double>> n_cells(new_table_size);

  for (int i=0;i<table_size;++i) {
    std::pair<utils::Array<std::string>, double>& line = lval.cells_[i];
    bool matches = true;
    /* Checks if line matches with valuation. */
    for (int j=0;j<ind_size;++j)
      if (line.first[indices[j]] != inst[j].second)
        matches = false;
    /* If it matches, create new line with line-valuation. */
    if (matches) {
      utils::Array<std::string> new_line(m_indices.Size());
      int old_line_size = line.first.Size();

      /* Gets all values that are not contained in the valuation scope. */
      for (l=0;l<new_line.Size();++l)
        new_line[l] = line.first[m_indices[l]];

      n_cells[k++] = std::make_pair(new_line, line.second);
    }
  }

  return Factor(m_scope, n_cells);
}

namespace {
  
}

Factor Factor::MultiplyCopy(const Factor& lval, const Factor& rval) {
  utils::Array<Variable> new_scope(utils::Union(lval.scope_, rval.scope_));
  utils::Array<std::pair<utils::Array<std::string>, double>> n_cells;

  int k, l=k=0;
  int sc = new_scope.Size();
  utils::Array<int> ptrs(sx, 0);
  for (int i=sc-1;i>=0;--i) {
    n_cells[k] = std::make_pair(utils::Array<std::string>(sc), 1);
    /* Generate combinations. */
    for (int p=0;p<sc;++p) {
      int i_val = ptrs[p];
      n_cells[k].first[p] = new_scope[p].Values()[i_val];
      /* Updates all pointers. */
      if (ptrs[sc-1] >= new_scope[sc-1].Size()-1) {
        ptrs[sc-1] = 0, ++ptrs[sc-2];
        for (int q=sc-2;q>0;--q)
          if (ptrs[q] >= new_scope[q].Size()-1) {
            ptrs[q] = 0, ++ptrs[q-1];
          }
        /* If this is true, then all combinations have been generated. */
        if (ptrs[0] >= new_scope[0].Size()-1)
          break;
      }
    }
    /* Multiply values. */
    n_cells[k].second = lval.Find
  }
}
