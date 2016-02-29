#include "variable.h"

#include <string>

template<typename T>
Variable<T>::Variable(std::string name, T val) : name_(name), val_(val) {}

template<typename T>
Variable<T>::~Variable(void) {}
