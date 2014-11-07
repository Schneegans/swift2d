////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/AnimatedGuiSpriteComponent.hpp>

#include <swift2d/gui/AnimatedGuiShader.hpp>
#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/graphics/Pipeline.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

AnimatedGuiSpriteComponent::AnimatedGuiSpriteComponent()
  : GuiSpriteComponent()
  , Time(0.f, 1.0f, 10.0f)
  , UseRenderThreadTime(false) {}

////////////////////////////////////////////////////////////////////////////////

void AnimatedGuiSpriteComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    o.Texture->bind(ctx, 0);
    AnimatedGuiShader::get().use(ctx);

    math::vec2 size(
      1.0 * o.Size.x() / ctx.window_size.x(),
      1.0 * o.Size.y() / ctx.window_size.y()
    );

    math::vec2 offset(
      (2.0 * o.Offset.x() + o.Anchor.x() * (ctx.window_size.x() - o.Size.x()))/ctx.window_size.x(),
      (2.0 * o.Offset.y() + o.Anchor.y() * (ctx.window_size.y() - o.Size.y()))/ctx.window_size.y()
    );

    if (o.UseRenderThreadTime) {
      AnimatedGuiShader::get().time.Set(ctx.pipeline->get_total_time() - (int)ctx.pipeline->get_total_time());
    } else {
      AnimatedGuiShader::get().time.Set(o.Time);
    }
    AnimatedGuiShader::get().size.Set(size);
    AnimatedGuiShader::get().opacity.Set(o.Opacity);
    AnimatedGuiShader::get().offset.Set(offset);
    AnimatedGuiShader::get().diffuse.Set(0);
    Quad::get().draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedGuiSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth    = Depth();
  s.Opacity  = Opacity();
  s.Size     = Size();
  s.Anchor   = Anchor();
  s.Offset   = Offset();
  s.Texture  = Texture();
  s.Time     = Time();
  s.UseRenderThreadTime = UseRenderThreadTime();
  scene->renderers().animated_gui_sprite_elements.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedGuiSpriteComponent::accept(SavableObjectVisitor& visitor) {
  GuiSpriteComponent::accept(visitor);
  visitor.add_object("Time", Time);
  visitor.add_member("UseRenderThreadTime", UseRenderThreadTime);
}

////////////////////////////////////////////////////////////////////////////////

}
