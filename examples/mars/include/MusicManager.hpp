////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_MUSIC_MANAGER_HPP
#define MARS_MUSIC_MANAGER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

class MusicManager : public swift::Singleton<MusicManager> {
 public:

  void play_next();
  void play_prev();
  void pause();

  friend class swift::Singleton<MusicManager>;

 private:
  MusicManager();
  ~MusicManager() {};

  void play_track(int which);

  swift::SoundComponentPtr     music_;
  std::vector<swift::SoundPtr> tracks_;
  int                          current_track_;
};

// -----------------------------------------------------------------------------

#endif // MARS_MUSIC_MANAGER_HPP
