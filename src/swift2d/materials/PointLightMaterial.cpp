////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/PointLightMaterial.hpp>

#include <swift2d/geometries/Quad.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointLightMaterial::PointLightMaterial()
  : Color(swift::Color(1.f, 1.f, 1.f)) {}

////////////////////////////////////////////////////////////////////////////////

void PointLightMaterial::draw_quad(RenderContext const& ctx,
                               math::mat3 const& object_transform,
                               float object_depth) {

  // disable rotation
  auto transform(math::make_translate(math::get_translate(object_transform)));
  transform = transform * math::make_scale(math::get_scale(object_transform));

  Texture()->bind(ctx, 0);
  PointLightShader::instance()->use(ctx);
  PointLightShader::instance()->projection.Set(ctx.projection_matrix);
  PointLightShader::instance()->transform.Set(transform);
  PointLightShader::instance()->depth.Set(object_depth);
  PointLightShader::instance()->parallax.Set(ctx.projection_parallax);
  PointLightShader::instance()->screen_size.Set(ctx.size);
  PointLightShader::instance()->g_buffer_diffuse.Set(1);
  PointLightShader::instance()->g_buffer_normal.Set(2);
  PointLightShader::instance()->g_buffer_light.Set(3);
  PointLightShader::instance()->light_tex.Set(0);
  PointLightShader::instance()->light_color.Set(Color().vec3());

  Quad::instance()->draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void PointLightMaterial::accept(SavableObjectVisitor& visitor) {
  Material::accept(visitor);
  visitor.add_object("Texture", Texture);
  visitor.add_member("Color", Color);
}

////////////////////////////////////////////////////////////////////////////////

}
