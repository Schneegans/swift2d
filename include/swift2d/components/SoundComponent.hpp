////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SOUND_COMPONENT_HPP
#define SWIFT2D_SOUND_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/audio/Sound.hpp>
#include <swift2d/openal.hpp>
#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class AudioContext;

// shared pointer type definition ----------------------------------------------
class SoundComponent;
typedef std::shared_ptr<SoundComponent>       SoundComponentPtr;
typedef std::shared_ptr<const SoundComponent> ConstSoundComponentPtr;

// -----------------------------------------------------------------------------
class SoundComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  // ---------------------------------------------------------------- properties
  SoundProperty Sound;
  Float         Volume;

  // ---------------------------------------------------- construction interface
  SoundComponent()
    : source_(new oalplus::Source()) {

    Sound.on_change().connect([&](SoundPtr const& val) {
      stop();
      source_->Buffer(val->get_buffer());
    });

    Volume.on_change().connect([&](float val) {
      source_->Gain(val);
    });
  }

  ~SoundComponent() {
    stop();
    source_->DetachBuffers();
    delete source_;
  }

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static SoundComponentPtr create(Args&& ... a) {
    return std::make_shared<SoundComponent>(a...);
  }

  // creates a copy from this
  SoundComponentPtr create_copy() const {
    return std::make_shared<SoundComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "SoundComponent"; }

  virtual void update(double time) {
    TransformableComponent::update(time);
    auto pos(get_world_position());
    source_->Position(pos.x(), pos.y(), 0);
  }

  virtual void play() {
    source_->Play();
  }

  virtual void pause() {
    source_->Pause();
  }


  virtual void stop() {
    source_->Stop();
  }

  virtual void accept(SavableObjectVisitor& visitor) {
    TransformableComponent::accept(visitor);
    visitor.add_member("Volume", Volume);
    visitor.add_object("Sound", Sound);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  oalplus::Source* source_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_SOUND_COMPONENT_HPP
