////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SOUND_HPP
#define SWIFT2D_SOUND_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/audio/AudioBuffer.hpp>
#include <swift2d/properties.hpp>

#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Sound;
typedef std::shared_ptr<Sound>       SoundPtr;
typedef std::shared_ptr<const Sound> ConstSoundPtr;
typedef Property<SoundPtr>           SoundProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL Sound : public AudioBuffer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  String FileName;

  static SoundPtr create() {
    return std::make_shared<Sound>();
  }

  static SoundPtr create(std::string const& file_name) {
    auto sound(std::make_shared<Sound>());
    sound->FileName = file_name;
    return sound;
  }

  Sound();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Sound"; }

  void load(oalplus::Source* source);
  void unload(oalplus::Source* source);

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 protected:
  void load_from_file(std::string const& file_name);

 private:
  oalplus::Buffer buffer_;

};

}

#endif // SWIFT2D_SOUND_HPP
