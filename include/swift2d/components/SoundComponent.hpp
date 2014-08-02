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
  SoundComponent();
  ~SoundComponent();

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

#endif  // SWIFT2D_SOUND_COMPONENT_HPP
