#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <string>
#include <initializer_list>
#include <vector>

/* Variable
 *
 * The variable name is the name of the Variable.
 * The variable vals are the possible (discrete) values Variable may take.
 *
 */
class Variable {
  public:
    Variable(std::string name, std::initializer_list<std::string> vals);
    ~Variable(void);

    const std::string& Name(void) const { return name_; }
    const std::vector<std::string>& Values(void) const { return vals_; }

    int NumVals(void) const { return vals_.size(); }
  private:
    std::string name_;
    std::vector<std::string> vals_;
};

#endif
