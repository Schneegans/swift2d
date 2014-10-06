////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TrailSystemComponent.hpp>

#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TrailSystemComponent::TrailSystemComponent()
  : MaxCount(1000)
  , Life(1.0)
  , Depth(0.f)
  , StartWidth(1.f),               EndWidth(1.f)
  , StartGlow(0.f),                EndGlow(0.f)
  , StartColor(Color(1, 1, 1, 1)), EndColor(Color(1, 1, 1, 1))
  , TextureRepeat(1.f)
  , UseGlobalTexCoords(false)
  , BlendAdd(false)
  , trail_system_(TrailSystem::create(MaxCount())) {

  MaxCount.on_change().connect([&](int val){
    trail_system_->set_max_trail_points(val);
  });
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystemComponent::add_emitter(TrailEmitterComponent const* emitter) {
  emitters_.insert(emitter);
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystemComponent::remove_emitter(TrailEmitterComponent const* emitter) {
  emitters_.erase(emitter);
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystemComponent::serialize(SerializedScenePtr& scene) const {

  Serialized s;

  s.Depth = Depth();
  s.Life = Life();
  s.StartWidth = StartWidth();
  s.EndWidth = EndWidth();
  s.StartGlow = StartGlow();
  s.EndGlow = EndGlow();
  s.StartColor = StartColor().vec4();
  s.EndColor = EndColor().vec4();
  s.Texture = Texture();
  s.TextureRepeat = TextureRepeat();
  s.UseGlobalTexCoords = UseGlobalTexCoords();
  s.BlendAdd = BlendAdd();
  s.System = trail_system_;
  s.Emitters.reserve(emitters_.size());

  for (auto const& emitter: emitters_) {
    s.Emitters.push_back(emitter->make_serialized_emitter());
  }

  scene->renderers().trail_system_renderer.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystemComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("MaxCount",      MaxCount);
  visitor.add_member("Depth",         Depth);
  visitor.add_member("Life",          Life);
  visitor.add_member("StartWidth",    StartWidth);
  visitor.add_member("EndWidth",      EndWidth);
  visitor.add_member("StartGlow",     StartGlow);
  visitor.add_member("EndGlow",       EndGlow);
  visitor.add_member("StartColor",    StartColor);
  visitor.add_member("EndColor",      EndColor);
  visitor.add_object("Texture",       Texture);
  visitor.add_member("TextureRepeat", TextureRepeat);
  visitor.add_member("UseGlobalTexCoords", UseGlobalTexCoords);
  visitor.add_member("BlendAdd",      BlendAdd);
}

////////////////////////////////////////////////////////////////////////////////

}
