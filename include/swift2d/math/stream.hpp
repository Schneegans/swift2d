////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATH_STREAM_HPP
#define SWIFT2D_MATH_STREAM_HPP

// includes  -------------------------------------------------------------------
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <ostream>

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, glm::detail::tvec2<T> const& vec) {

  stream << "(" << vec.x << ", " << vec.y << ")";

  return stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, glm::detail::tvec3<T> const& vec) {

  stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";

  return stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, glm::detail::tvec4<T> const& vec) {

  stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.x << ")";

  return stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, glm::detail::tmat2x2<T> const& mat) {

  stream << "(" << glm::row(mat, 0)[0] << ", " << glm::row(mat, 0)[1] << "," << std::endl;
  stream << " " << glm::row(mat, 1)[0] << ", " << glm::row(mat, 1)[1] << ")";

  return stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, glm::detail::tmat3x3<T> const& mat) {

  stream << "(" << glm::row(mat, 0)[0] << ", " << glm::row(mat, 0)[1] << ", " << glm::row(mat, 0)[2] << ", " << std::endl;
  stream << " " << glm::row(mat, 1)[0] << ", " << glm::row(mat, 1)[1] << ", " << glm::row(mat, 1)[2] << ", " << std::endl;
  stream << " " << glm::row(mat, 2)[0] << ", " << glm::row(mat, 2)[1] << ", " << glm::row(mat, 2)[2] << ")";

  return stream;
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::ostream& operator<<(std::ostream& stream, glm::detail::tmat4x4<T> const& mat) {

  stream << "(" << glm::row(mat, 0)[0] << ", " << glm::row(mat, 0)[1] << ", " << glm::row(mat, 0)[2] << ", " << glm::row(mat, 0)[3] << "," << std::endl;
  stream << " " << glm::row(mat, 1)[0] << ", " << glm::row(mat, 1)[1] << ", " << glm::row(mat, 1)[2] << ", " << glm::row(mat, 1)[3] << "," << std::endl;
  stream << " " << glm::row(mat, 2)[0] << ", " << glm::row(mat, 2)[1] << ", " << glm::row(mat, 2)[2] << ", " << glm::row(mat, 2)[3] << "," << std::endl;
  stream << " " << glm::row(mat, 3)[0] << ", " << glm::row(mat, 3)[1] << ", " << glm::row(mat, 3)[2] << ", " << glm::row(mat, 3)[3] << ")";

  return stream;
}

////////////////////////////////////////////////////////////////////////////////

#endif //SWIFT2D_MATH_STREAM_HPP
