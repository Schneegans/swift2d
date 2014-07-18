////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MUSIC_HPP
#define SWIFT2D_MUSIC_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/audio/Sound.hpp>

#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Music;
typedef std::shared_ptr<Music>       MusicPtr;
typedef std::shared_ptr<const Music> ConstMusicPtr;
typedef Property<MusicPtr>           MusicProperty;

// -----------------------------------------------------------------------------
class Music : public Sound {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  static MusicPtr create() {
    return std::make_shared<Music>();
  }

  static MusicPtr create_from_file(std::string const& file_name) {
    auto Music(std::make_shared<Music>());
    Music->load_from_file(file_name);
    return Music;
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Music"; }

  std::wstring const& get_title();
  std::wstring const& get_artist();
  std::wstring const& get_album();
  std::wstring const& get_year();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 protected:
  void load_from_file(std::string const& file_name);

 private:
  std::wstring title_;
  std::wstring artist_;
  std::wstring album_;
  std::wstring year_;
};

}

#endif // SWIFT2D_MUSIC_HPP
