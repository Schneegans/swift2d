////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
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
  : Color(swift::Color(1.f, 1.f, 1.f)) {}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::update(double time) {
  TransformableComponent::update(time);
  DepthComponent::update(time, get_user());
}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::serialize(SerializedScenePtr& scene) const {
  auto transform(math::make_translation(math::get_translation(WorldTransform())));
  transform = transform * math::make_scale(math::get_scale(WorldTransform()));

  Serialized s;
  s.Depth       = WorldDepth();
  s.Transform   = transform;
  s.Texture     = Texture();
  s.Color       = Color().vec4();
  scene->renderers().point_lights.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  DepthComponent::accept(visitor);
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

    ogl::Context::BlendFunc(
      oglplus::BlendFunction::One,
      oglplus::BlendFunction::One
    );

    auto& shader(PointLightShader::get());

    object.Texture->bind(ctx, 3);
    shader.use();
    shader.projection.Set(ctx.projection_matrix);
    shader.depth.Set(object.Depth);
    shader.parallax.Set(ctx.projection_parallax);
    shader.screen_size.Set(ctx.g_buffer_size/(ctx.light_sub_sampling ? 2 : 1));
    shader.g_buffer_normal.Set(1);
    shader.g_buffer_light.Set(2);
    shader.light_tex.Set(3);

    int index(0);

    while (index < transforms.size()) {
      std::size_t count(std::min(100, (int)transforms.size()-index));

      shader.transform.Set(count, (const math::mat3*)&transforms[index]);
      shader.light_color.Set(count, (const math::vec4*)&colors[index]);

      Quad::get().draw(count);

      index += count;
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
