////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_STL_HELPERS_HPP
#define SWIFT2D_STL_HELPERS_HPP

// includes  -------------------------------------------------------------------
#include <vector>
#include <iostream>

namespace std {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
T from_string(std::string const& v) {
  std::istringstream iss(v);
  T result;
  iss >> result;
  return result;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v) {

  typename std::vector<T>::const_iterator i(v.begin());
  while (i != v.end()) {
      os << *i;

      if (++i != v.end()) {
        os << " ";
      }
  }
  return os;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) {
  v.clear();

  T new_one;
  while (is >> new_one) {
    v.push_back(new_one);
  }

  is.clear();

  return is;
}

////////////////////////////////////////////////////////////////////////////////

}
#endif //SWIFT2D_STL_HELPERS_HPP
