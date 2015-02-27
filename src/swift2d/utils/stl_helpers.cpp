////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/utils/stl_helpers.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> split_string(std::string const& s, char delim) {
  std::vector<std::string> elems;

  std::stringstream ss(s);
  std::string item;

  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }

  return elems;
}

////////////////////////////////////////////////////////////////////////////////

bool string_contains(std::string const& s, char c) {
  return s.find(c) != std::string::npos;
}

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

namespace RakNet {

std::ostream& operator<<(std::ostream& os, RakString const& v) {
  os << v.C_String();
  return os;
}

////////////////////////////////////////////////////////////////////////////////

std::istream& operator>>(std::istream& is, RakString& v) {
  std::string tmp;
  is >> tmp;
  v = tmp.c_str();
  return is;
}

////////////////////////////////////////////////////////////////////////////////

}
