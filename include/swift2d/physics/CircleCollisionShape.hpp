////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_CIRCLE_COLLISION_SHAPE_HPP
#define SWIFT2D_CIRCLE_COLLISION_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/physics/CollisionShape.hpp>

// forward declares ------------------------------------------------------------
class b2Shape;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class CircleCollisionShape;
typedef std::shared_ptr<CircleCollisionShape>       CircleCollisionShapePtr;
typedef std::shared_ptr<const CircleCollisionShape> ConstCircleCollisionShapePtr;
typedef Property<CircleCollisionShapePtr>           CircleCollisionShapeProperty;

// -----------------------------------------------------------------------------
class CircleCollisionShape : public CollisionShape {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  float Radius;

  // ----------------------------------------------------- contruction interface
  static CircleCollisionShapePtr create() {
    return std::make_shared<CircleCollisionShape>();
  }

  CircleCollisionShape();

  // ------------------------------------------------------------ public methods
  b2Shape* get_shape(math::mat3 const& body_transform) const;

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "CircleCollisionShape"; }
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_CIRCLE_COLLISION_SHAPE_HPP
