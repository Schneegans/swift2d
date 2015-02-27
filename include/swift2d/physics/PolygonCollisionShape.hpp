////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_POLYGON_COLLISION_SHAPE_HPP
#define SWIFT2D_POLYGON_COLLISION_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/physics/CollisionShape.hpp>

// forward declares ------------------------------------------------------------
class b2Shape;

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class PolygonCollisionShape;
typedef std::shared_ptr<PolygonCollisionShape>       PolygonCollisionShapePtr;
typedef std::shared_ptr<const PolygonCollisionShape> ConstPolygonCollisionShapePtr;
typedef Property<PolygonCollisionShapePtr>           PolygonCollisionShapeProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL PolygonCollisionShape : public CollisionShape {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  std::vector<math::vec2> Coordinates;

  // ----------------------------------------------------- contruction interface
  static PolygonCollisionShapePtr create() {
    return std::make_shared<PolygonCollisionShape>();
  }

  PolygonCollisionShape();

  // ------------------------------------------------------------ public methods
  b2Shape* get_shape(math::mat3 const& body_transform) const;

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "PolygonCollisionShape"; }
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_POLYGON_COLLISION_SHAPE_HPP
