////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TrailSystemComponent.hpp>
#include <swift2d/trails/TexturedTrailShader.hpp>
#include <swift2d/trails/ColoredTrailShader.hpp>

#include <gl.h>
#include <glext.h>
#include <glew.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TrailSystemComponent::TrailSystemComponent()
  : MaxCount(1000)
  , Depth(0.f)
  , StartWidth(1.f),               EndWidth(1.f)
  , StartGlow(0.f),                EndGlow(0.f)
  , StartColor(Color(1, 1, 1, 1)), EndColor(Color(1, 1, 1, 1))
  , TextureRepeat(1.f)
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

void TrailSystemComponent::draw(RenderContext const& ctx) {
  trail_system_->update_trails(serialized_emitters_, ctx);

  ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::One);

  if (Texture()) {
    auto& shader(TexturedTrailShader::get());
    shader.use(ctx);
    Texture()->bind(ctx, 0);
    shader.texture.Set(0);
    shader.texture_repeat.Set(TextureRepeat());
    shader.projection.            Set(ctx.projection_matrix);
    shader.start_width.           Set(StartWidth());
    shader.end_width.             Set(EndWidth());
    shader.start_color.           Set(StartColor().vec4());
    shader.end_color.             Set(EndColor().vec4());
    shader.glow.                  Set(math::vec2(StartGlow(), EndGlow()));
  } else {
    auto& shader(ColoredTrailShader::get());
    shader.use(ctx);
    shader.projection.            Set(ctx.projection_matrix);
    shader.start_width.           Set(StartWidth());
    shader.end_width.             Set(EndWidth());
    shader.start_color.           Set(StartColor().vec4());
    shader.end_color.             Set(EndColor().vec4());
    shader.glow.                  Set(math::vec2(StartGlow(), EndGlow()));
  }

  trail_system_->draw_trails(serialized_emitters_, ctx);

  ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::OneMinusSrcAlpha);
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystemComponent::serialize(SerializedScenePtr& scene) const {
  serialized_emitters_.clear();
  for (auto const& emitter: emitters_) {
    serialized_emitters_.push_back(emitter->make_serialized_emitter());
  }

  scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystemComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("MaxCount",      MaxCount);
  visitor.add_member("Depth",         Depth);
  visitor.add_member("StartWidth",    StartWidth);
  visitor.add_member("EndWidth",      EndWidth);
  visitor.add_member("StartGlow",     StartGlow);
  visitor.add_member("EndGlow",       EndGlow);
  visitor.add_member("StartColor",    StartColor);
  visitor.add_member("EndColor",      EndColor);
  visitor.add_object("Texture",       Texture);
  visitor.add_member("TextureRepeat", TextureRepeat);
}

////////////////////////////////////////////////////////////////////////////////

}
