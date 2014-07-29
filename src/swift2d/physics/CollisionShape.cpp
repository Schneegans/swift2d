////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/CollisionShape.hpp>

#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/math.hpp>

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

b2Shape* CircleCollisionShape::get_shape(math::mat3 const& body_transform) const {
  float scale(math::get_scale(body_transform).x());

  auto shape(new b2CircleShape());
  shape->m_radius = Radius * scale;
  return shape;
}

void CircleCollisionShape::accept(SavableObjectVisitor& visitor) {
  CollisionShape::accept(visitor);
  visitor.add_member("Radius", Radius);
}

////////////////////////////////////////////////////////////////////////////////

b2Shape* BoxCollisionShape::get_shape(math::mat3 const& body_transform) const {
  auto scale(math::get_scale(body_transform));

  auto shape(new b2PolygonShape());
  shape->SetAsBox(Width*scale.x(), Height*scale.y());
  return shape;
}

void BoxCollisionShape::accept(SavableObjectVisitor& visitor) {
  CollisionShape::accept(visitor);
  visitor.add_member("Width", Width);
  visitor.add_member("Height", Height);
}

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

}
