////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATH_HELPERS_HPP
#define SWIFT2D_MATH_HELPERS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/math/types.hpp>

#include <algorithm>

namespace swift {
namespace math {

template<typename T>
inline T clamp(T n, T min, T max) {
  return std::max(min, std::min(n, max));
}

}
}

#endif // SWIFT2D_MATH_HELPERS_HPP
