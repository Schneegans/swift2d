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
#include <swift2d/objects/SavableObjectVisitor.hpp>
#include <swift2d/openal.hpp>
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
class Sound : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  static SoundPtr create() {
    return std::make_shared<Sound>();
  }

  static SoundPtr create_from_file(std::string const& file_name) {
    auto sound(std::make_shared<Sound>());
    sound->load_from_file(file_name);
    return sound;
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Sound"; }

  oalplus::Buffer const& get_buffer() const { return buffer_; }

  std::string const& get_title();
  std::string const& get_artist();
  std::string const& get_album();
  std::string const& get_year();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 protected:
  void load_from_file(std::string const& file_name);

 private:
  std::string title_;
  std::string artist_;
  std::string album_;
  std::string year_;

  oalplus::Buffer buffer_;

};

}

#endif // SWIFT2D_SOUND_HPP
