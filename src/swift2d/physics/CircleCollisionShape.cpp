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

// includes  -------------------------------------------------------------------
#include <swift2d/physics/CircleCollisionShape.hpp>

#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/math.hpp>

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

CircleCollisionShape::CircleCollisionShape()
  : Radius(1.f) {}

////////////////////////////////////////////////////////////////////////////////

b2Shape* CircleCollisionShape::get_shape(math::mat3 const& body_transform) const {
  float scale(math::get_scale(body_transform).x());

  auto shape(new b2CircleShape());
  shape->m_radius = Radius * scale;

  return shape;
}

////////////////////////////////////////////////////////////////////////////////

void CircleCollisionShape::accept(SavableObjectVisitor& visitor) {
  CollisionShape::accept(visitor);
  visitor.add_member("Radius", Radius);
}

////////////////////////////////////////////////////////////////////////////////

}
