////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_BOX_COLLISION_SHAPE_HPP
#define SWIFT2D_BOX_COLLISION_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/physics/CollisionShape.hpp>

// forward declares ------------------------------------------------------------
class b2Shape;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class BoxCollisionShape;
typedef std::shared_ptr<BoxCollisionShape>       BoxCollisionShapePtr;
typedef std::shared_ptr<const BoxCollisionShape> ConstBoxCollisionShapePtr;
typedef Property<BoxCollisionShapePtr>           BoxCollisionShapeProperty;

// -----------------------------------------------------------------------------
class BoxCollisionShape : public CollisionShape {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  float Width;
  float Height;

  // ----------------------------------------------------- contruction interface
  static BoxCollisionShapePtr create() {
    return std::make_shared<BoxCollisionShape>();
  }

  BoxCollisionShape();

  // ------------------------------------------------------------ public methods
  b2Shape* get_shape(math::mat3 const& body_transform) const;

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "BoxCollisionShape"; }
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_BOX_COLLISION_SHAPE_HPP
