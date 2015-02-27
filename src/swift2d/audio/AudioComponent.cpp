////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/audio/AudioComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

AudioComponent::AudioComponent()
  : Volume(1.0)
  , Pitch(1.0)
  , Loop(false)
  , State(INITIAL)
  , source_(new oalplus::Source()) {

  source_->ReferenceDistance(10);

  Sound.on_change().connect([&](AudioBufferPtr const& val) {
    stop();
    return true;
  });

  Volume.on_change().connect([&](float val) {
    source_->Gain(val);
    return true;
  });

  Pitch.on_change().connect([&](float val) {
    source_->Pitch(val);
    return true;
  });

  Loop.on_change().connect([&](bool val) {
    source_->Looping(val);
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

AudioComponent::~AudioComponent() {
  stop();
  source_->DetachBuffers();
  delete source_;
}

////////////////////////////////////////////////////////////////////////////////

void AudioComponent::update(double time) {
  TransformableComponent::update(time);
  auto pos(get_world_position());
  source_->Position(pos.x(), pos.y(), 0);

  if      (source_->State() == oalplus::SourceState::Playing) State = PLAYING;
  else if (source_->State() == oalplus::SourceState::Paused)  State = PAUSED;
  else if (source_->State() == oalplus::SourceState::Stopped) {
    int queued(0);
    alGetSourcei(oalplus::GetALName(*source_), AL_BUFFERS_QUEUED, &queued);
    if (queued == 0) State = INTERUPTED;
    else             State = FINISHED;
  }

  if (Sound()) {
    Sound()->update(source_, time);
  }
}

////////////////////////////////////////////////////////////////////////////////

void AudioComponent::play() {
  Sound()->load(source_);
  source_->Play();
}

////////////////////////////////////////////////////////////////////////////////

void AudioComponent::pause() {
  if (source_->State() == oalplus::SourceState::Playing) source_->Pause();
  else                                                   source_->Play();
}

////////////////////////////////////////////////////////////////////////////////

void AudioComponent::stop() {
  source_->Stop();

  if (Sound()) {
    Sound()->unload(source_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void AudioComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_object_property("Sound", Sound);
  visitor.add_member("Volume", Volume);
  visitor.add_member("Pitch", Pitch);
  visitor.add_member("Loop", Loop);
}

////////////////////////////////////////////////////////////////////////////////

}
