#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <string>

/** Variable
 */
template<typename T> class Variable {
  public:
    Variable(std::string name, T val);
    ~Variable(void);

    const std::string& Name(void) const { return name; }
    const T& Value(void) const { return val; }

    void Value(T n_val) { val = n_val; }
  private:
    std::string name_;
    T val_;
}

#endif
