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
class TrailSystemComponent;
typedef std::shared_ptr<TrailSystemComponent>       TrailSystemComponentPtr;
typedef Property<TrailSystemComponentPtr>           TrailSystemComponentProperty;

class TrailEmitterComponent;
typedef std::shared_ptr<TrailEmitterComponent>       TrailEmitterComponentPtr;
typedef std::shared_ptr<const TrailEmitterComponent> ConstTrailEmitterComponentPtr;
typedef Property<TrailEmitterComponentPtr>           TrailEmitterComponentProperty;

struct SWIFT_DLL TrailSegment {
  math::vec2 Position;
  float      TimeSinceLastSpawn;
  float      TimeSincePrev1Spawn;
  float      TimeSincePrev2Spawn;
  float      Life;
  float      StartAge;
  math::vec2 Prev1Position;
  math::vec2 Prev2Position;
  math::vec2 Prev3Position;
  math::vec2 Prev4Position;
};

// -----------------------------------------------------------------------------
class SWIFT_DLL TrailEmitterComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  TrailSystemComponentProperty TrailSystem;
  Float MaxSpawnGap;
  Float MinSpawnGap;

  Float Life;
  Float StartAge;

  Bool  Paused;

  // ----------------------------------------------------- contruction interface
  TrailEmitterComponent();
  ~TrailEmitterComponent();

  template <typename... Args>
  static TrailEmitterComponentPtr create(Args&& ... a) {
    return std::make_shared<TrailEmitterComponent>(a...);
  }

  // creates a copy from this
  TrailEmitterComponentPtr create_copy() const {
    return std::make_shared<TrailEmitterComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "TrailEmitterComponent"; }

  void on_detach(double time);

  virtual void update(double time);
  virtual void accept(SavableObjectVisitor& visitor);

  friend class TrailSystemComponent;

 private:
  void spawn_segment();
  TrailSegment make_end_segment() const;

  float      time_since_last_spawn_;
  float      time_since_prev_1_spawn_;
  float      time_since_prev_2_spawn_;
  math::vec2 prev_1_position_;
  math::vec2 prev_2_position_;
  math::vec2 prev_3_position_;
  math::vec2 prev_4_position_;

  bool first_frame_;
};

}

#endif // SWIFT2D_TRAIL_EMITTER_HPP
