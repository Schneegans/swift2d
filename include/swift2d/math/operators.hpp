////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATH_OPERATORS_HPP
#define SWIFT2D_MATH_OPERATORS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/math/types.hpp>
#include <swift2d/utils/platform.hpp>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace oglplus {

////////////////////////////////////////////////////////////////////////////////

template<typename T, std::size_t C>
oglplus::Vector<T, C> operator*(oglplus::Vector<T, C> const& a, oglplus::Vector<T, C> const& b) {

  Vector<T, C> result;

  for (unsigned c = 0; c < C; ++c) {
    result[c] = a[c] * b[c];
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T, std::size_t C>
Vector<T, C> operator/(Vector<T, C> const& a, Vector<T, C> const& b) {

  Vector<T, C> result;

  for (unsigned c = 0; c < C; ++c) {
    result[c] = a[c] / b[c];
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T, std::size_t C>
std::ostream& operator<<(std::ostream& out_stream, Vector<T, C> const& vec) {

  out_stream << "(";
  for (unsigned c = 0; c < C; ++c) {
    out_stream << (c != 0 ? " " : "") << vec[c];
  }
  out_stream << ")";

  return out_stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T, std::size_t C>
std::istream& operator>>(std::istream& in_stream, Vector<T, C>& vec) {

  Vector<T, C> tmp_vec;
  std::istream::char_type cur_char(0);
  bool bracket_version(false);

  in_stream >> cur_char;

  bracket_version = (cur_char == std::istream::char_type('('));

  if (!bracket_version) {
    in_stream.putback(cur_char);
  }

  for (unsigned c = 0; c < C; ++c) {
    in_stream >> tmp_vec[c];
  }

  if (bracket_version) {
    in_stream >> cur_char;
    if (cur_char != std::istream::char_type(')')) {
      in_stream.clear(std::ios_base::badbit);
    }
  }

  if (in_stream) {
    vec = tmp_vec;
  }

  return in_stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T, std::size_t R, std::size_t C>
std::ostream& operator<<(std::ostream& out_stream, Matrix<T, R, C> const& mat) {

  out_stream << "(";
  for (unsigned r = 0; r < R; ++r) {
    for (unsigned c = 0; c < C; ++c) {
      unsigned out_index = r + c * R;
      out_stream << (out_index != 0 ? " " : "") << mat.At(r, c);
    }

    if (r != R - 1) {
      out_stream << std::endl;
    }
  }
  out_stream << ")";

  return (out_stream);
}

////////////////////////////////////////////////////////////////////////////////

template<typename T, std::size_t R, std::size_t C>
std::istream& operator>>(std::istream& in_stream, Matrix<T, R, C>& mat) {

  Matrix<T, R, C> tmp_mat;
  std::istream::char_type cur_char(0);
  bool bracket_version(false);

  in_stream >> cur_char;

  bracket_version = (cur_char == std::istream::char_type('('));

  if (!bracket_version) {
    in_stream.putback(cur_char);
  }
  for (unsigned r = 0; r < R; ++r) {
    for (unsigned c = 0; c < C; ++c) {
      T val;
      in_stream >> val;
      tmp_mat.Set(r, c, val);
    }
  }

  if (bracket_version) {
    in_stream >> cur_char;
    if (cur_char != std::istream::char_type(')')) {
      in_stream.clear(std::ios_base::badbit);
    }
  }

  if (in_stream) {
    mat = tmp_mat;
  }

  return in_stream;
}


////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

#endif //SWIFT2D_MATH_OPERATORS_HPP
