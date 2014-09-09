////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/SoundComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SoundComponent::SoundComponent()
  : Volume(1.0)
  , Pitch(1.0)
  , source_(new oalplus::Source()) {

  Sound.on_change().connect([&](SoundPtr const& val) {
    stop();
    source_->Buffer(val->get_buffer());
  });

  Volume.on_change().connect([&](float val) {
    source_->Gain(val);
  });

  Pitch.on_change().connect([&](float val) {
    source_->Pitch(val);
  });
}

////////////////////////////////////////////////////////////////////////////////

SoundComponent::~SoundComponent() {
  stop();
  source_->DetachBuffers();
  delete source_;
}

////////////////////////////////////////////////////////////////////////////////

void SoundComponent::update(double time) {
  TransformableComponent::update(time);
  auto pos(get_world_position());
  source_->Position(pos.x(), pos.y(), 0);
}

////////////////////////////////////////////////////////////////////////////////

void SoundComponent::play() {
  source_->Play();
}

////////////////////////////////////////////////////////////////////////////////

void SoundComponent::pause() {
  if (source_->State() == oalplus::SourceState::Playing) source_->Pause();
  else                                                   source_->Play();
}

////////////////////////////////////////////////////////////////////////////////

void SoundComponent::stop() {
  source_->Stop();
}

////////////////////////////////////////////////////////////////////////////////

void SoundComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Volume", Volume);
  visitor.add_object("Sound", Sound);
}

////////////////////////////////////////////////////////////////////////////////

}
