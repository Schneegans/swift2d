////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
