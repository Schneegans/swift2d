////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_COLLISION_SHAPE_HPP
#define SWIFT2D_COLLISION_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/utils/SavableObject.hpp>
#include <swift2d/properties.hpp>

// forward declares ------------------------------------------------------------
class b2Shape;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class CollisionShape;
typedef std::shared_ptr<CollisionShape>       CollisionShapePtr;
typedef std::shared_ptr<const CollisionShape> ConstCollisionShapePtr;
typedef Property<CollisionShapePtr>           CollisionShapeProperty;

// -----------------------------------------------------------------------------
class CollisionShape : public SavableObject {
 public:
  virtual b2Shape* get_shape(math::mat3 const& body_transform) const = 0;
};

// shared pointer type definition ----------------------------------------------
class CircleCollisionShape;
typedef std::shared_ptr<CircleCollisionShape>       CircleCollisionShapePtr;
typedef std::shared_ptr<const CircleCollisionShape> ConstCircleCollisionShapePtr;
typedef Property<CircleCollisionShapePtr>           CircleCollisionShapeProperty;

// -----------------------------------------------------------------------------
class CircleCollisionShape : public CollisionShape {
 public:

  float Radius;

  static CircleCollisionShapePtr create() {
    return std::make_shared<CircleCollisionShape>();
  }

  CircleCollisionShape()
    : Radius(1.f) {}

  b2Shape* get_shape(math::mat3 const& body_transform) const;

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "CircleCollisionShape"; }
  virtual void accept(SavableObjectVisitor& visitor);
};

// shared pointer type definition ----------------------------------------------
class BoxCollisionShape;
typedef std::shared_ptr<BoxCollisionShape>       BoxCollisionShapePtr;
typedef std::shared_ptr<const BoxCollisionShape> ConstBoxCollisionShapePtr;
typedef Property<BoxCollisionShapePtr>           BoxCollisionShapeProperty;

// -----------------------------------------------------------------------------
class BoxCollisionShape : public CollisionShape {
 public:

  float Width;
  float Height;

  static BoxCollisionShapePtr create() {
    return std::make_shared<BoxCollisionShape>();
  }

  BoxCollisionShape()
    : Width(1.f)
    , Height(1.f) {}

  b2Shape* get_shape(math::mat3 const& body_transform) const;

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "BoxCollisionShape"; }
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_COLLISION_SHAPE_HPP
