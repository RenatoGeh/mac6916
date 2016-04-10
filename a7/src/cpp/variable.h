#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <string>
#include <initializer_list>
#include <utility>

#include "utils.h"

/** A Variable
 *
 * name: variable's name
 * vals: variable's possible values
 */
class Variable {
  public:
    Variable(std::string name, std::initializer_list<std::string> vals);
    ~Variable(void);

    const std::string& Name(void) const { return name_; }

    const utils::Array<std::string>& Values(void) const { return vals_; }
    int Size(void) const { return vals_.Size(); }

    /* The index of instanciation query on vals_, an ordered set of all instanciations. */
    int Index(std::string query) const;

    friend bool operator<(const Variable& lval, const Variable& rval) {
      return lval.Name().compare(rval.Name())<0;
    }
    friend bool operator>(const Variable& lval, const Variable& rval) {
      return lval.Name().compare(rval.Name())>0;
    }
    friend bool operator==(const Variable& lval, const Variable& rval) {
      return !lval.Name().compare(rval.Name());
    }
    friend bool operator<=(const Variable& lval, const Variable& rval) {
      return lval.Name().compare(rval.Name())<=0;
    }
    friend bool operator>=(const Variable& lval, const Variable& rval) {
      return lval.Name().compare(rval.Name())>=0;
    }
  private:
    std::string name_;
    utils::Array<std::string> vals_;
};

#endif
