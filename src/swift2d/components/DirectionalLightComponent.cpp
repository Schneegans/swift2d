////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/DirectionalLightComponent.hpp>

#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/materials/DirectionalLightShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DirectionalLightComponent::DirectionalLightComponent()
  : Direction(math::vec3(0, 0, -1))
  , Color(swift::Color(1.f, 1.f, 1.f)) {}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth       = -std::numeric_limits<float>::max();
  s.Direction   = Direction();
  s.Color       = Color().vec4();
  scene->renderers().directional_lights.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Direction", Direction);
  visitor.add_member("Color", Color);
}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::Renderer::draw_no_objects(RenderContext const& ctx) {
  // draw only emissive if there are no sun lights
  draw(ctx, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {

  std::vector<math::vec3> light_dirs;
  std::vector<math::vec4> light_colors;

  for (int i(start); i<end; ++i) {
    if (light_dirs.size() < 10) {
      light_dirs.push_back(objects[i].Direction);
      light_colors.push_back(objects[i].Color);
    }
  }

  SWIFT_PUSH_GL_RANGE("Draw SunLights");

  ctx.gl.Disable(oglplus::Capability::Blend);

  auto& shader(DirectionalLightShader::get());
  shader.use(ctx);

  if (light_dirs.size() > 0) {
    shader.light_dirs.Set(light_dirs);
    shader.light_colors.Set(light_colors);
  }
  shader.light_count.Set((int)light_dirs.size());
  shader.g_buffer_normal.Set(1);
  shader.g_buffer_light.Set(2);

  Quad::get().draw(ctx);

  ctx.gl.Enable(oglplus::Capability::Blend);

  SWIFT_POP_GL_RANGE();
}

////////////////////////////////////////////////////////////////////////////////

}
