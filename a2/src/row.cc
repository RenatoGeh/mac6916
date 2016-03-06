#include "row.h"

#include <initializer_list>
#include <vector>
#include <string>
#include <algorithm>

#include "variable.h"

Row::Row(std::initializer_list<std::string> vals, double pr) : pr_(pr) {
  for (auto v : vals)
    vals_.push_back(v);
}

Row::~Row(void) {}

void Row::Init(std::vector<Variable>& scope) {
  int i=0;
  for (auto it=vals_.begin();it!=vals_.end();++it,++i) {
    std::string pre = (scope.begin()+i)->Name() + "=";
    it->insert(0, pre);
  }

  std::sort(vals_.begin()+1, vals_.end());
  compact_ = Row::Compact(vals_);
}

double Row::Find(std::initializer_list<std::string> vals, std::vector<Row>& cpt) {
  std::sort(vals.begin()+1, vals.end());
  std::string inst(Row::Compact(vals));
  for (auto it=cpt.begin();it!=cpt.end();++it) {
    std::string vals(it->CompactValue());
    if (!vals.compare(inst))
      return it->Pr();
  }
  return -1;
}

double Row::Find(std::vector<std::string> vals, std::vector<Row>& cpt) {
  std::sort(vals.begin()+1, vals.end());
  std::string inst(Row::Compact(vals));
  for (auto it=cpt.begin();it!=cpt.end();++it) {
    std::string vals(it->CompactValue());
    if (!vals.compare(inst))
      return it->Pr();
  }
  return -1;
}

std::string Row::Compact(std::initializer_list<std::string> inst) {
  std::sort(inst.begin()+1, inst.end());
  std::string comp(*inst.begin());

  for (auto it=inst.begin()+1;it!=inst.end();++it)
    comp += "," + *it;

  return comp;
}

std::string Row::Compact(std::vector<std::string> inst) {
  std::sort(inst.begin()+1, inst.end());
  std::string comp(*inst.begin());

  for (auto it=inst.begin()+1;it!=inst.end();++it)
    comp += "," + *it;

  return comp;
}

bool Row::Equals(std::string cval, Variable var, std::string val) {
  std::string cmp = var.Name()+"="+val;
  return !cmp.compare(cval);
}
