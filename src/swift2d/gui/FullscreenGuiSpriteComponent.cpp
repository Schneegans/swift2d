////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/FullscreenGuiSpriteComponent.hpp>

#include <swift2d/gui/GuiShader.hpp>
#include <swift2d/graphics/WindowManager.hpp>
#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/graphics/Pipeline.hpp>
#include <swift2d/settings.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

FullscreenGuiSpriteComponent::FullscreenGuiSpriteComponent()
  : Opacity(1)
  , Texture(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void FullscreenGuiSpriteComponent::update(double time) {
  Component::update(time);
  DepthComponent::update(time, get_user());
}

////////////////////////////////////////////////////////////////////////////////

void FullscreenGuiSpriteComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    o.Texture->bind(ctx, 0);
    GuiShader::get().use();

    math::vec2 size(
      1.0 * o.Size.x() / ctx.window_size.x(),
      1.0 * o.Size.y() / ctx.window_size.y()
    );

    math::vec3 offset(
      (2.0 * o.Offset.x() + o.Anchor.x() * (ctx.window_size.x() - o.Size.x()))/ctx.window_size.x(),
      (2.0 * o.Offset.y() + o.Anchor.y() * (ctx.window_size.y() - o.Size.y()))/ctx.window_size.y(),
      0
    );

    GuiShader::get().size.Set(size);
    GuiShader::get().opacity.Set(o.Opacity);
    GuiShader::get().offset_rot.Set(offset);
    GuiShader::get().diffuse.Set(0);
    Quad::get().draw();
  }
}

////////////////////////////////////////////////////////////////////////////////

void FullscreenGuiSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth    = WorldDepth();
  s.Opacity  = Opacity();
  s.Size     = SettingsWrapper::get().Settings->WindowSize();
  s.Anchor   = math::vec2(0.f, 0.f);
  s.Offset   = math::vec2(0.f, 0.f);
  s.Texture  = Texture();
  scene->renderers().gui_sprite_elements.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void FullscreenGuiSpriteComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  DepthComponent::accept(visitor);
  visitor.add_member("Opacity",          Opacity);
  visitor.add_object_property("Texture", Texture);
}

////////////////////////////////////////////////////////////////////////////////

}
