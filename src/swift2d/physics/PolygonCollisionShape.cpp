////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/PolygonCollisionShape.hpp>

#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/stl_helpers.hpp>

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PolygonCollisionShape::PolygonCollisionShape() {}

////////////////////////////////////////////////////////////////////////////////

b2Shape* PolygonCollisionShape::get_shape(math::mat3 const& body_transform) const {
  auto scale(math::get_scale(body_transform));

  std::vector<b2Vec2> vertices(Coordinates.size());
  for (int i(0); i<Coordinates.size(); ++i) {
    vertices[i] = b2Vec2(Coordinates[i].x()*scale.x(), Coordinates[i].y()*scale.y());
  }

  auto shape(new b2PolygonShape());
  shape->Set(vertices.data(), Coordinates.size());

  return shape;
}

////////////////////////////////////////////////////////////////////////////////

void PolygonCollisionShape::accept(SavableObjectVisitor& visitor) {
  CollisionShape::accept(visitor);
  visitor.add_member("Coordinates", Coordinates);
}

////////////////////////////////////////////////////////////////////////////////

}
