////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/FullscreenGuiSpriteComponent.hpp>

#include <swift2d/gui/GuiShader.hpp>
#include <swift2d/graphics/WindowManager.hpp>
#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/graphics/Pipeline.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

FullscreenGuiSpriteComponent::FullscreenGuiSpriteComponent()
  : Depth(0.f)
  , Opacity(1)
  , Texture(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void FullscreenGuiSpriteComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    o.Texture->bind(ctx, 0);
    GuiShader::get().use(ctx);

    math::vec2 size(
      1.0 * o.Size.x() / ctx.window_size.x(),
      1.0 * o.Size.y() / ctx.window_size.y()
    );

    math::vec2 offset(
      (2.0 * o.Offset.x() + o.Anchor.x() * (ctx.window_size.x() - o.Size.x()))/ctx.window_size.x(),
      (2.0 * o.Offset.y() + o.Anchor.y() * (ctx.window_size.y() - o.Size.y()))/ctx.window_size.y()
    );

    GuiShader::get().size.Set(size);
    GuiShader::get().opacity.Set(o.Opacity);
    GuiShader::get().offset.Set(offset);
    GuiShader::get().diffuse.Set(0);
    Quad::get().draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void FullscreenGuiSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth    = Depth();
  s.Opacity  = Opacity();
  s.Size     = WindowManager::get().current()->Size();
  s.Anchor   = math::vec2(0.f, 0.f);
  s.Offset   = math::vec2(0.f, 0.f);
  s.Texture  = Texture();
  scene->renderers().gui_sprite_elements.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void FullscreenGuiSpriteComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("Depth",            Depth);
  visitor.add_member("Opacity",          Opacity);
  visitor.add_object_property("Texture", Texture);
}

////////////////////////////////////////////////////////////////////////////////

}