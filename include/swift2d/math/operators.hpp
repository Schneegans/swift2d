////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATH_OPERATORS_HPP
#define SWIFT2D_MATH_OPERATORS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/math/types.hpp>

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, oglplus::Vector<T, 2> const& vec) {
  stream << "(" << vec.x() << ", " << vec.y() << ")";
  return stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, oglplus::Vector<T, 3> const& vec) {
  stream << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
  return stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, oglplus::Matrix<T, 2, 2> const& mat) {

  stream << "(" << mat.At(0,0) << ", " << mat.At(0,1) << "," << std::endl;
  stream << " " << mat.At(1,0) << ", " << mat.At(1,1) << ")";

  return stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, oglplus::Matrix<T, 3, 3> const& mat) {

  stream << "(" << mat.At(0,0) << ", " << mat.At(0,1) << ", " << mat.At(0,2) << ", " << std::endl;
  stream << " " << mat.At(1,0) << ", " << mat.At(1,1) << ", " << mat.At(1,2) << ", " << std::endl;
  stream << " " << mat.At(2,0) << ", " << mat.At(2,1) << ", " << mat.At(2,2) << ")";

  return stream;
}

////////////////////////////////////////////////////////////////////////////////

#endif //SWIFT2D_MATH_OPERATORS_HPP
