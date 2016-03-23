#ifndef UTILS_H_
#define UTILS_H_

#include <initializer_list>
#include <utility>
#include <algorithm>

namespace utils {
  /* An array. */
  template<typename T> class Array {
    public:
      Array(void) : a_(nullptr), s_(0) {}
      Array(T *a, int s) : a_(a), s_(s) {}
      Array(std::initializer_list<T> a) : s_(a.size()) {
        a_ = new T[s_];
        int i=0;
        for (auto it=a.begin();it!=a.end();++it,++i)
          a_[i] = *it;
      }
      Array(int s) : a_(new T[s]), s_(s) {
        for (int i=0;i<s_;++i)
          a_[i] = nullptr;
      }

      ~Array(void) {
        delete[] a_;
      }

      int Size(void) const { return s_; }
      T* Vals(void) const { return a_; }

      T* Begin(void) const { return a_; }
      T* End(void) const { return a_ + s_; }

      /* Expects i to not be out of bounds. */
      T& operator[](int i) const { return a_[i]; }

    private:
      T *a_;
      int s_;
  };

  /* A matrix. */
  template<typename T> class Matrix {
    public:
      Matrix(Array<T> *v, int s) : v_(v), n_(s) { m_ = n_>0?v[0].Size():0; }
      Matrix(std::initializer_list<std::initializer_list<T>> m) : n_(m.size()) {
        v_ = new Array<T>[n_];
        int i=0;
        for (auto it=m.begin();it!=m.end();++it,++i)
          v_[i] = new Array<T>(*it);
      }
      Matrix(int n, int m) : v_(new Array<T>[n]), n_(n), m_(m) {
        for (int i=0;i<n_;++i)
          v_[i] = nullptr;
      }

      ~Matrix(void) {
        for (int i=0;i<n_;++i)
          delete v_[i];
        delete v_;
      }

      std::pair<int, int> Size(void) const { return std::make_pair(n_, m_); }
      int Lines(void) const { return n_; }
      int Columns(void) const { return m_; }
      Array<T>* Vals(void) const { return v_; }

      Array<T>* Begin(void) const { return v_; }
      Array<T>* End(void) const { return v_ + n_; }

      /* Expects i to not be out of bounds. */
      T& operator[](int i) { return v_[i]; }

    private:
      Array<T> *v_;
      int n_, m_;
  };

  template<typename T> inline void Sort(Array<T> data) {
    std::sort(data.Begin(), data.End());
  }

  template<typename T> inline T* BinarySearch(Array<T> data, T query) {
    return BinarySearch(data.Begin(), data.End(), query);
  }

  /* Finds element query in an ordered strictly increasing range (begin, end) in O(logn) and
   * returns index.
   *
   * Returns iterator if found; else end.
   */
  template<class Iterator, typename T>
  Iterator BinarySearch(Iterator begin, Iterator end, T query);

  /* Finds the intersection of two ordered sets in linear O(n+m) time, where n is the size of the
   * first array and m the size of the second. The result is the ordered intersection set of the
   * two sets
  */
  template<typename T> Array<T> Intersection(const Array<T>& lval, const Array<T>& rval) {
    int min_sets, max_sets;

    if (lval.Size() >= rval.Size())
      max_sets = lval.Size(), min_sets = rval.Size();
    else
      max_sets = rval.Size(), min_sets = lval.Size();

    int k, i, j=i=k=0;
    Array<T> aux(max_sets);

    while(std::max(i, j) < min_sets) {
      if (lval[i] < rval[j])
        ++i;
      else if (lval[i] == rval[j])
        aux[k++] = lval[i++], ++j;
      else
        ++j;
    }

    Array<T> trimmed(k);
    for (i=0;i<k;++i)
      trimmed[i] = aux[i];

    return trimmed;
  }

  /* Finds the union of two ordered sets in linear time O(n+m), where n is the size of the first
   * set and m the size of the second. The result is the ordered union set of the two sets.
   */
  template<typename T> Array<T> Union(const Array<T>& lval, const Array<T>& rval) {
    int min_set, max_set;
    int discr; /* discr := |lval|>|rval|?1:|lval|==|rval|?0:-1; */

    int ls = lval.Size(), rs = rval.Size();
    if (ls >= rs)
      max_set = ls, min_set = rs, discr = ls>rs?1:0;
    else
      max_set = rs, min_set = ls, discr = -1;

    Array<T> aux(max_set + min_set);

    int i, j, k=i=j=0;
    while (std::max(i, j) < min_set) {
      if (lval[i] == rval[j])
        aux[k++] = lval[i++], ++j;
      else if (lval[i] < rval[j])
        aux[k++] = lval[i++];
      else
        aux[k++] = rval[j++];
    }

    if (discr > 0)
      while (i<ls)
        aux[k++] = lval[i++];
    else if (discr < 0)
      while (j<rs)
        aux[k++] = rval[j++];

    Array<T> trimmed(k);

    for (i=0;i<k;++i)
      trimmed[i] = aux[i];

    return trimmed;
  }

  /* Set minus.
   * Note: rval must be a subset of lval.
   */
  template<typename T> Array<T> SubMinus(const Array<T>& lval, const Array<T>& rval) {
    int n = lval.Size(), m = rval.Size();
    Array<T> res(std::max(n, m) - std::min(n, m));

    int k=0, l=0;
    for (int i=0;i<n;++i)
      if (lval[i] != rval[k])
        res[l++] = lval[i];
      else ++k;

    return res;
  }

  /* Set minus.
   * Note: rval can be any set.
   */
  template<typename T> Array<T> Minus(const Array<T>& lval, const Array<T>& rval) {
    return  SubMinus(lval, Intersection(rval));
  }
}

#endif
