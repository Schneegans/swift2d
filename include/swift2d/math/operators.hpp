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
#include <iosfwd>

////////////////////////////////////////////////////////////////////////////////

namespace oglplus {

////////////////////////////////////////////////////////////////////////////////

template<typename T, std::size_t C>
Vector<T, C> operator*(Vector<T, C> const& a, Vector<T, C> const& b);

template<typename T, std::size_t C>
Vector<T, C> operator/(Vector<T, C> const& a, Vector<T, C> const& b);

template<typename T, std::size_t C>
std::ostream& operator<<(std::ostream& out_stream, Vector<T, C> const& vec);

template<typename T, std::size_t C>
std::istream& operator>>(std::istream& in_stream, Vector<T, C>& vec);

template<typename T, std::size_t R, std::size_t C>
std::ostream& operator<<(std::ostream& out_stream, Matrix<T, R, C> const& mat);

template<typename T, std::size_t R, std::size_t C>
std::istream& operator>>(std::istream& in_stream, Matrix<T, R, C>& mat);

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

#endif //SWIFT2D_MATH_OPERATORS_HPP
