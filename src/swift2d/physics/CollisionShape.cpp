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

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

b2Shape* CircleCollisionShape::get_shape() const {
  auto shape(new b2CircleShape());
  shape->m_radius = radius;
  return shape;
}

void CircleCollisionShape::accept(SavableObjectVisitor& visitor) {
  CollisionShape::accept(visitor);
  visitor.add_member("Radius", radius);
}

////////////////////////////////////////////////////////////////////////////////

b2Shape* BoxCollisionShape::get_shape() const {
  auto shape(new b2PolygonShape());
  shape->SetAsBox(width, height);
  return shape;
}

void BoxCollisionShape::accept(SavableObjectVisitor& visitor) {
  CollisionShape::accept(visitor);
  visitor.add_member("Width", width);
  visitor.add_member("Height", height);
}

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

}
