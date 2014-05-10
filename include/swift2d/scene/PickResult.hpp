////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_PICK_RESULT_HPP
#define SWIFT2D_PICK_RESULT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/math.hpp>

namespace swift {

// forward declares ------------------------------------------------------------
class Node;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
struct PickResult {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  enum Options { PICK_ALL                 = 0,
                 PICK_ONLY_FIRST_OBJECT   = 1<<1,
                 PICK_ONLY_FIRST_FACE     = 1<<2,
                 GET_WORLD_NORMALS        = 1<<3};

  PickResult(float d, Node* o, math::vec3 const& n, bool w)
    : distance(d), object(o), normal(n), is_world_normal(w) {}

  float             distance;
  Node*             object;
  mutable math::vec3 normal;
  mutable bool      is_world_normal;

  bool operator<(PickResult const& lhs) const {
    return distance < lhs.distance;
  }
};

}

#endif  // SWIFT2D_PICK_RESULT_HPP
