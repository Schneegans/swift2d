////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TRAIL_EMITTER_HPP
#define SWIFT2D_TRAIL_EMITTER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/scene/SerializedScene.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TrailEmitterComponent;
typedef std::shared_ptr<TrailEmitterComponent>       TrailEmitterComponentPtr;
typedef std::shared_ptr<const TrailEmitterComponent> ConstTrailEmitterComponentPtr;
typedef Property<TrailEmitterComponentPtr>           TrailEmitterComponentProperty;

struct SerializedTrailEmitter {
  float Life;
  float Density;

  math::mat3 WorldTransform;
  math::mat3 Prev1WorldTransform;
  math::mat3 Prev2WorldTransform;
  math::mat3 Prev3WorldTransform;
  TrailEmitterComponent const* Self;
};

// -----------------------------------------------------------------------------
class TrailEmitterComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float Life;

  Float Density;

  // ----------------------------------------------------- contruction interface
  TrailEmitterComponent();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "TrailEmitterComponent"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);

  SerializedTrailEmitter make_serialized_emitter() const;

 private:

  math::mat3 prev_1_world_transform_;
  math::mat3 prev_2_world_transform_;
  math::mat3 prev_3_world_transform_;

  bool first_frame_;

};

}

#endif // SWIFT2D_TRAIL_EMITTER_HPP
