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

// header
#include <swift2d/math/random.hpp>

// external headers
#include <ctime>

namespace swift {
namespace math {
namespace random {
namespace {

////////////////////////////////////////////////////////////////////////////////

unsigned int init_seed() {

  unsigned int seed = static_cast<unsigned int>(std::time(nullptr));
  std::srand(seed);
  return seed;
}

unsigned int global_seed = init_seed();

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

void set_seed(unsigned int seed) {

  std::srand(seed);
  global_seed = seed;
}

////////////////////////////////////////////////////////////////////////////////

unsigned int get_seed() { return global_seed; }

////////////////////////////////////////////////////////////////////////////////

}
}
}
