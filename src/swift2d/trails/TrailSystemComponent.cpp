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
#include <swift2d/graphics/Pipeline.hpp>
#include <swift2d/trails/TrailSystem.hpp>
#include <swift2d/trails/TexturedTrailShader.hpp>
#include <swift2d/trails/ColoredTrailShader.hpp>

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
    return true;
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

  scene->renderers().trail_systems.add(std::move(s));
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
  visitor.add_object_property("Texture",       Texture);
  visitor.add_member("TextureRepeat", TextureRepeat);
  visitor.add_member("UseGlobalTexCoords", UseGlobalTexCoords);
  visitor.add_member("BlendAdd",      BlendAdd);
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystemComponent::Renderer::predraw(RenderContext const& ctx) {
  for (auto& object : objects) {
    SWIFT_PUSH_GL_RANGE("Update TrailSystem");
    object.System->update_trails(object.Emitters, object, ctx);
    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystemComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {

  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    SWIFT_PUSH_GL_RANGE("Draw TrailSystem");

    double total_time(ctx.pipeline->get_total_time());

    if (o.BlendAdd) {
      ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::One);
    }

    if (o.Texture) {
      auto& shader(TexturedTrailShader::get());
      shader.use(ctx);
      o.Texture->bind(ctx, 0);
      shader.texture.Set(0);
      shader.texture_repeat.        Set(o.TextureRepeat);
      shader.projection.            Set(ctx.projection_matrix);
      shader.start_width.           Set(o.StartWidth);
      shader.end_width.             Set(o.EndWidth);
      shader.start_color.           Set(o.StartColor);
      shader.end_color.             Set(o.EndColor);
      shader.glow.                  Set(math::vec2(o.StartGlow, o.EndGlow));
      shader.use_global_texcoords.  Set(o.UseGlobalTexCoords ? 1 : 0);
      shader.total_time.            Set(total_time * 1000.0);
    } else {
      auto& shader(ColoredTrailShader::get());
      shader.use(ctx);
      shader.projection.            Set(ctx.projection_matrix);
      shader.start_width.           Set(o.StartWidth);
      shader.end_width.             Set(o.EndWidth);
      shader.start_color.           Set(o.StartColor);
      shader.end_color.             Set(o.EndColor);
      shader.glow.                  Set(math::vec2(o.StartGlow, o.EndGlow));
      shader.use_global_texcoords.  Set(o.UseGlobalTexCoords ? 1 : 0);
      shader.total_time.            Set(total_time * 1000.0);
    }

    o.System->draw_trails(o.Emitters, o, ctx);

    if (o.BlendAdd) {
      ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::OneMinusSrcAlpha);
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
