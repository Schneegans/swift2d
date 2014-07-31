////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/BoxCollisionShape.hpp>

#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/math.hpp>

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

BoxCollisionShape::BoxCollisionShape()
  : Width(1.f)
  , Height(1.f) {}

////////////////////////////////////////////////////////////////////////////////

b2Shape* BoxCollisionShape::get_shape(math::mat3 const& body_transform) const {
  auto scale(math::get_scale(body_transform));

  auto shape(new b2PolygonShape());
  shape->SetAsBox(Width*scale.x(), Height*scale.y());
  return shape;
}

////////////////////////////////////////////////////////////////////////////////

void BoxCollisionShape::accept(SavableObjectVisitor& visitor) {
  CollisionShape::accept(visitor);
  visitor.add_member("Width", Width);
  visitor.add_member("Height", Height);
}

////////////////////////////////////////////////////////////////////////////////

}
