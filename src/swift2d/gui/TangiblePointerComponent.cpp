////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/gui/TangiblePointerComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/gui/GuiShader.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TangiblePointerComponent::TangiblePointerComponent()
  : Component()
  , DepthComponent()
  , Texture(nullptr)
  , Size(math::vec2i(50,50))
  , MinDistance(0.8)
  , MaxDistance(3) {}

////////////////////////////////////////////////////////////////////////////////

void TangiblePointerComponent::update(double time) {
  Component::update(time);
  DepthComponent::update(time, get_user());
}

////////////////////////////////////////////////////////////////////////////////

void TangiblePointerComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    o.Texture->bind(ctx, 0);
    GuiShader::get().use();

    math::vec2 size(
      1.0 * o.Size.x() / ctx.window_size.x(),
      1.0 * o.Size.y() / ctx.window_size.y()
    );

    auto pos = ctx.projection_matrix * math::vec3(o.Position.x(), o.Position.y(), 1.f);
    pos /= pos.z();

    float dist = math::get_length(pos.xy());
    float rot = math::get_rotation(pos.xy());

    if (dist > o.MinDistance && dist < o.MaxDistance) {
      float opacity;

      if (dist < 1.f) {
        opacity = std::min(1.f, std::max(0.f, (dist-o.MinDistance)/(1.f-o.MinDistance)));
      } else {
        opacity = 1.f - std::min(1.f, std::max(0.f, (dist-1.f)/(o.MaxDistance-1.f)));
      }

      auto screen_space_pos = pos.xy() / dist * o.MinDistance;

      GuiShader::get().size.Set(size);
      GuiShader::get().opacity.Set(opacity);
      GuiShader::get().offset_rot.Set(math::vec3(screen_space_pos.x(), screen_space_pos.y(), rot));
      GuiShader::get().diffuse.Set(0);
      Quad::get().draw();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void TangiblePointerComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth    = WorldDepth();
  s.Texture  = Texture;
  s.Size     = Size();
  s.MinDistance   = MinDistance();
  s.MaxDistance   = MaxDistance();
  s.Position = get_user()->get_world_position();
  scene->renderers().tangible_pointers.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void TangiblePointerComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  DepthComponent::accept(visitor);
  visitor.add_object_ptr("Texture", Texture);
  visitor.add_member("Size",        Size);
  visitor.add_member("MinDistance", MinDistance);
  visitor.add_member("MaxDistance", MaxDistance);
}

////////////////////////////////////////////////////////////////////////////////

}
