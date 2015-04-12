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
class SWIFT_DLL TransformableComponent : public Component {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Mat3 Transform;
  Mat3 WorldTransform;

  // ------------------------------------------------------------ public methods
  virtual void on_init();
  virtual void update(double time);
  virtual void update_world_transform();

  // -------------------------------------------------- transformation interface
  virtual void scale     (math::vec2 const& scale);
  virtual void scale     (float scale);
  virtual void scale     (float x, float y);
  virtual void rotate    (float angle);
  virtual void translate (math::vec2 const& delta);
  virtual void translate (float x, float y);

  virtual float      get_rotation() const;
  virtual float      get_world_rotation() const;
  virtual math::vec2 get_direction() const;
  virtual math::vec2 get_world_direction() const;
  virtual math::vec2 get_position() const;
  virtual math::vec2 get_world_position() const;

  virtual void accept(SavableObjectVisitor& visitor);
};

}

#endif  // SWIFT2D_TRANSFORMABLE_COMPONENT_HPP
