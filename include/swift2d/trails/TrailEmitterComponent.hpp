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
  float MaxSpawnGap;
  float MinSpawnGap;

  math::vec2 Position;
  float      TimeSinceLastSpawn;
  float      TimeSincePrev1Spawn;
  math::vec2 LastPosition;
  math::vec2 Prev1Position;
  math::vec2 Prev2Position;
  math::vec2 Prev3Position;
  bool       SpawnNewPoint;
  TrailEmitterComponent const* Self;
};

// -----------------------------------------------------------------------------
class TrailEmitterComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float Life;

  Float MaxSpawnGap;
  Float MinSpawnGap;

  // ----------------------------------------------------- contruction interface
  TrailEmitterComponent();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "TrailEmitterComponent"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);

  SerializedTrailEmitter make_serialized_emitter() const;

 private:

  math::vec2 position_;
  float      time_since_last_spawn_;
  float      time_since_prev_1_spawn_;
  math::vec2 last_position_;
  math::vec2 prev_1_position_;
  math::vec2 prev_2_position_;
  math::vec2 prev_3_position_;

  bool first_frame_;
  bool spawn_new_point_;

};

}

#endif // SWIFT2D_TRAIL_EMITTER_HPP
