////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/GuiSpriteComponent.hpp>

#include <swift2d/gui/GuiShader.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GuiSpriteComponent::GuiSpriteComponent()
  : Depth(0.f)
  , Size(math::vec2i(10,10))
  , Anchor(math::vec2i(0,0))
  , Texture(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

void GuiSpriteComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
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
    GuiShader::get().offset.Set(offset);
    GuiShader::get().diffuse.Set(0);
    Quad::get().draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void GuiSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth    = Depth();
  s.Size     = Size();
  s.Anchor   = Anchor();
  s.Offset   = Offset();
  s.Texture = Texture();
  scene->renderers().gui_sprite_elements.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void GuiSpriteComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("Depth",            Depth);
  visitor.add_member("Size",             Size);
  visitor.add_member("Anchor",           Anchor);
  visitor.add_member("Offset",           Offset);
  visitor.add_object_property("Texture", Texture);
}

////////////////////////////////////////////////////////////////////////////////

}
