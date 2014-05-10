////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATH_RANDOM_HPP
#define SWIFT2D_MATH_RANDOM_HPP

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
float get(float begin, float end);

/**
 * Returns a random integer value.
 *
 * \param   begin Smallest possible result.
 * \param   end   Largets possible result.
 *
 * \return        A random integer value.
 */
int get(int begin, int end);
}
}
}

#endif // SWIFT2D_MATH_RANDOM_HPP

