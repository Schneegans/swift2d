////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/PointLightComponent.hpp>

#include <swift2d/materials/PointLightShader.hpp>
#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/geometries/Quad.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointLightComponent::PointLightComponent()
  : Depth(0.f)
  , Color(swift::Color(1.f, 1.f, 1.f)) {}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::serialize(SerializedScenePtr& scene) const {
  auto transform(math::make_translation(math::get_translation(WorldTransform())));
  transform = transform * math::make_scale(math::get_scale(WorldTransform()));

  Serialized s;
  s.Depth       = Depth();
  s.Transform   = transform;
  s.Texture     = Texture();
  s.Color       = Color().vec4();
  scene->renderers().point_lights.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_object_property("Texture", Texture);
  visitor.add_member("Color", Color);
}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  std::sort(objects.begin() + start, objects.begin() + end,
    [](PointLightComponent::Serialized const& a, PointLightComponent::Serialized const& b){
      return a.Texture.get() < b.Texture.get();
    });

  while (start < end) {

    auto& object(objects[start]);
    auto& tex(object.Texture);

    SWIFT_PUSH_GL_RANGE("Draw PointLight");

    std::vector<math::mat3> transforms;
    std::vector<math::vec4> colors;

    while (start < end && objects[start].Texture == tex) {
      transforms.push_back(objects[start].Transform);
      colors.push_back(objects[start].Color);
      ++start;
    }

    ctx.gl.BlendFunc(
      oglplus::BlendFunction::One,
      oglplus::BlendFunction::One
    );

    auto& shader(PointLightShader::get());

    object.Texture->bind(ctx, 3);
    shader.use(ctx);
    shader.projection.Set(ctx.projection_matrix);
    shader.depth.Set(object.Depth);
    shader.parallax.Set(ctx.projection_parallax);
    shader.screen_size.Set(ctx.g_buffer_size/(ctx.light_sub_sampling ? 2 : 1));
    shader.g_buffer_normal.Set(1);
    shader.g_buffer_light.Set(2);
    shader.light_tex.Set(3);

    int index(0);

    while (index < transforms.size()) {
      int count(std::min(100, (int)transforms.size()-index));

      shader.transform.Set(std::vector<math::mat3>(transforms.begin() + index, transforms.begin() + index + count));
      shader.light_color.Set(std::vector<math::vec4>(colors.begin() + index, colors.begin() + index + count));

      Quad::get().draw(ctx, count);

      index += count;
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
