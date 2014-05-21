////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef FIBRGLASS_SOUND_HPP
#define FIBRGLASS_SOUND_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/openal.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// Stores geometry data. A mesh can be loaded from an Assimp mesh and the     //
// draw onto a context.                                                       //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Sound {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Sound() {};
  Sound(std::string const& file_name) {
    load_from_file(file_name);
  }

  // initializes the contained data from a given texture file.
  void load_from_file(std::string const& file_name) {
    oalplus::ALUtilityToolkit alut(false);
    buffer_ = alut.CreateBufferFromFile(file_name.c_str());
  }

  oalplus::Buffer const& get_buffer() const { return buffer_; }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  oalplus::Buffer buffer_;

};

}

#endif // FIBRGLASS_SOUND_HPP
