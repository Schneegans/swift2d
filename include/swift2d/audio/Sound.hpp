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
// Stores geometry data. A mesh can be loaded from an Assimp mesh and the     //
// draw onto a context.                                                       //
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

  virtual void accept(SavableObjectVisitor& visitor) {
    // visitor.add_member("Size", Size);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  void load_from_file(std::string const& file_name);

  oalplus::Buffer buffer_;

};

}

#endif // SWIFT2D_SOUND_HPP
