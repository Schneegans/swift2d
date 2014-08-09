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
#include <swift2d/geometries/Quad.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointLightComponent::PointLightComponent()
  : Depth(0.f)
  , Color(swift::Color(1.f, 1.f, 1.f)) {}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::draw(RenderContext const& ctx) {

  // disable rotation
  auto transform(math::make_translation(math::get_translation(WorldTransform())));
  transform = transform * math::make_scale(math::get_scale(WorldTransform()));

  Texture()->bind(ctx, 0);
  PointLightShader::instance()->use(ctx);
  PointLightShader::instance()->projection.Set(ctx.projection_matrix);
  PointLightShader::instance()->transform.Set(transform);
  PointLightShader::instance()->depth.Set(Depth());
  PointLightShader::instance()->parallax.Set(ctx.projection_parallax);
  PointLightShader::instance()->screen_size.Set(ctx.g_buffer_size);
  PointLightShader::instance()->g_buffer_diffuse.Set(1);
  PointLightShader::instance()->g_buffer_normal.Set(2);
  PointLightShader::instance()->g_buffer_light.Set(3);
  PointLightShader::instance()->light_tex.Set(0);
  PointLightShader::instance()->light_color.Set(Color().vec4());

  Quad::instance()->draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::serialize(SerializedScenePtr& scene) const {
  scene->lights.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void PointLightComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_object("Texture", Texture);
  visitor.add_member("Color", Color);
}

////////////////////////////////////////////////////////////////////////////////

}
