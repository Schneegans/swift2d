////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TRANSFORMABLE_COMPONENT_HPP
#define SWIFT2D_TRANSFORMABLE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/math.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TransformableComponent;
typedef std::shared_ptr<TransformableComponent>       TransformableComponentPtr;
typedef std::shared_ptr<const TransformableComponent> ConstTransformableComponentPtr;

// -----------------------------------------------------------------------------
class TransformableComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Mat3 pTransform;
  Mat3 pWorldTransform;

  // ------------------------------------------------------------ public methods
  virtual void update(double time);

  virtual math::vec2 get_position() const;
  virtual math::vec2 get_world_position() const;

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:
  TransformableComponent() {}

};

}

#endif  // SWIFT2D_TRANSFORMABLE_COMPONENT_HPP
