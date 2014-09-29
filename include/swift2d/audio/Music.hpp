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
#include <swift2d/audio/AudioBuffer.hpp>
#include <swift2d/audio/Jamendo.hpp>
#include <swift2d/utils/Downloader.hpp>
#include <swift2d/properties.hpp>

#include <mutex>
#include <queue>
#include <thread>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Music;
typedef std::shared_ptr<Music>       MusicPtr;
typedef std::shared_ptr<const Music> ConstMusicPtr;
typedef Property<MusicPtr>           MusicProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL Music : public AudioBuffer {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  template <typename... Args>
  static MusicPtr create(Args&& ... a) {
    return std::make_shared<Music>(a...);
  }

  Music();
  Music(std::string const& file_name);
  Music(Jamendo::Track const& track);
  virtual ~Music();

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Music"; }

  std::string const& get_title();
  std::string const& get_artist();
  std::string const& get_album();

  // internal interface --------------------------------------------------------
  void load(oalplus::Source* source);
  void unload(oalplus::Source* source);
  void update(oalplus::Source* source, double time);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  std::string file_;

  std::string title_;
  std::string artist_;
  std::string album_;
  std::string year_;
  int sample_rate_;
  int channels_;

  std::vector<ALuint> buffers_;
  unsigned long max_frames_;

  std::mutex load_mutex_;
  std::queue<std::vector<short>> buffer_queue_;
  int playing_id_;

  Downloader downloader_;
};

}

#endif // SWIFT2D_MUSIC_HPP
