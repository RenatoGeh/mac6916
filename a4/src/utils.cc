#include "utils.h"

#include <initializer_list>
#include <algorithm>

namespace utils {
  template<class Iterator, typename T>
  Iterator BinarySearch(Iterator begin, Iterator end, T query) {
    Iterator bottom = std::lower_bound(begin, end, query);

    if (bottom != end && !(query < *bottom))
      return bottom;
    else
      return end;
  }
}
