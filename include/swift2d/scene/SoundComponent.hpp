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
#include <swift2d/scene/TransformableComponent.hpp>

#include <swift2d/openal.hpp>
#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// A screen core represents a rectangular, virtual screen. Combined with a    //
// view core it defines the viewing frustum.                                  //
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

  SoundComponent() {
    oalplus::ALUtilityToolkit alut(false);
    buffer_ = alut.CreateBufferHelloWorld();
    source_.Buffer(buffer_);
  }

  ~SoundComponent() {
    source_.DetachBuffers();
  }

  // ------------------------------------------------------------ public methods
  virtual void update() {
    TransformableComponent::update();
    auto pos(get_world_position());
    source_.Position(pos.x(), pos.y(), 0);
  }

  virtual void play() {
    source_.Play();
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  oalplus::Source source_;
  oalplus::Buffer buffer_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_SOUND_COMPONENT_HPP
