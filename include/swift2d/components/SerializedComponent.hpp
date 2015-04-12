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

#ifndef SWIFT2D_SERIALIZED_COMPONENT_HPP
#define SWIFT2D_SERIALIZED_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/platform.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
struct SWIFT_DLL SerializedComponent {

  float Depth;

  bool operator<(SerializedComponent const& other) const {
    return Depth < other.Depth;
  }
};

}

#endif  // SWIFT2D_SERIALIZED_COMPONENT_HPP
