////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/MusicManager.hpp"
#include "../include/GuiManager.hpp"

using namespace swift;

////////////////////////////////////////////////////////////////////////////////

MusicManager::MusicManager()
  : current_track_(-1) {

  auto scene = SceneManager::instance()->get_default();

  music_ = scene->add<SoundComponent>();
  music_->Volume = 0.5f;

  tracks_.push_back(Sound::create_from_file(Application::instance()->get_resource("audio", "music.ogg")));
  tracks_.push_back(Sound::create_from_file(Application::instance()->get_resource("audio", "explorers.ogg")));
}

////////////////////////////////////////////////////////////////////////////////

void MusicManager::play_next() {
  play_track((current_track_+1)%tracks_.size());
}

////////////////////////////////////////////////////////////////////////////////

void MusicManager::play_prev() {
  play_track((current_track_-1 + tracks_.size())%tracks_.size());
}

////////////////////////////////////////////////////////////////////////////////

void MusicManager::pause() {
  music_->pause();
}

////////////////////////////////////////////////////////////////////////////////

void MusicManager::play_track(int which) {
  current_track_ = which;

  music_->stop();
  music_->Sound = tracks_[current_track_];
  music_->play();

  GuiManager::instance()->show_track_info(
    tracks_[current_track_]->get_title(),
    tracks_[current_track_]->get_artist(),
    tracks_[current_track_]->get_album()
  );
}

////////////////////////////////////////////////////////////////////////////////
