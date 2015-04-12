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

#ifndef SWIFT2D_COLLISION_SHAPE_HPP
#define SWIFT2D_COLLISION_SHAPE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/objects/SavableObject.hpp>
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
class SWIFT_DLL CollisionShape : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------ public methods
  virtual b2Shape* get_shape(math::mat3 const& body_transform) const = 0;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_COLLISION_SHAPE_HPP
