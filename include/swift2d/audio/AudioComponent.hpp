////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_AUDIO_COMPONENT_HPP
#define SWIFT2D_AUDIO_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/TransformableComponent.hpp>
#include <swift2d/audio/AudioBuffer.hpp>
#include <swift2d/openal.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// forward declares ------------------------------------------------------------
class AudioContext;

// shared pointer type definition ----------------------------------------------
class AudioComponent;
typedef std::shared_ptr<AudioComponent>       AudioComponentPtr;
typedef std::shared_ptr<const AudioComponent> ConstAudioComponentPtr;

// -----------------------------------------------------------------------------
class AudioComponent : public TransformableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  // ---------------------------------------------------------------- properties
  AudioBufferProperty Sound;
  Float               Volume;
  Float               Pitch;

  // ---------------------------------------------------- construction interface
  AudioComponent();
  ~AudioComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static AudioComponentPtr create(Args&& ... a) {
    return std::make_shared<AudioComponent>(a...);
  }

  // creates a copy from this
  AudioComponentPtr create_copy() const {
    return std::make_shared<AudioComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "AudioComponent"; }

  virtual void update(double time);

  virtual void play();
  virtual void pause();
  virtual void stop();

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  oalplus::Source* source_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_AUDIO_COMPONENT_HPP
