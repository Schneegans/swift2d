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

#ifndef SWIFT2D_MATH_RANDOM_HPP
#define SWIFT2D_MATH_RANDOM_HPP

#include <cstdlib>

namespace swift {
namespace math {
namespace random {

/**
 * Sets a new random seed.
 *
 * \param seed  The new seed.
 */
void set_seed(unsigned int seed);

/**
 * Gets the current seed.
 *
 * \return      The current seed.
 */
unsigned int get_seed();

/**
 * Returns a random floating point value.
 *
 * \param   begin Smallest possible result.
 * \param   end   Largets possible result.
 *
 * \return        A random floating point value.
 */
 template<typename T>
T get(T begin, T end) {
    return static_cast<T>(std::rand()) * (end - begin) / RAND_MAX + begin;
}


}
}
}

#endif // SWIFT2D_MATH_RANDOM_HPP

