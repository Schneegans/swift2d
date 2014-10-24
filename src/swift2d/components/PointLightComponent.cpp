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
  visitor.add_object("Texture", Texture);
  visitor.add_member("Color", Color);
}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  
  std::vector<math::vec3> light_pos_radius;
  std::vector<math::vec4> light_colors;

  for (int i(start); i<end; ++i) {
    if (light_pos_radius.size() < 100) {

      auto mat(ctx.projection_matrix * objects[i].Transform);

      math::vec3 pos(0.f, 0.f, 1.f);
      pos = mat * pos * std::pow(ctx.projection_parallax, objects[i].Depth);
      pos /= pos.z();

      math::vec3 dir(1.f, 0.f, 0.f);
      dir = mat * dir * std::pow(ctx.projection_parallax, objects[i].Depth);

      pos[2] = dir.x()*dir.x() + dir.y()*dir.y();

      light_pos_radius.push_back(pos);
      light_colors.push_back(objects[i].Color);
    } else {
      LOG_WARNING << "There can only be at most 100 point light sources in"
                  << " one scene!" << std::endl;
      break;
    }
  }

  std::cout << light_pos_radius.size()  << std::endl;

  if (light_pos_radius.size() > 0) {

    SWIFT_PUSH_GL_RANGE("Draw PointLights");

    ctx.gl.BlendFunc(
      oglplus::BlendFunction::One,
      oglplus::BlendFunction::One
    );

    auto& shader(PointLightShader::get());
    shader.light_pos_radius.Set(light_pos_radius);
    shader.light_colors.Set(light_colors);
    shader.light_count.Set(light_pos_radius.size());
    shader.g_buffer_normal.Set(1);
    shader.g_buffer_light.Set(2);
    shader.screen_size.Set(ctx.g_buffer_size);

    Quad::get().draw(ctx);

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
