////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/DirectionalLightMaterial.hpp>

#include <swift2d/geometries/Quad.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DirectionalLightMaterial::DirectionalLightMaterial(math::vec3 const& dir,
                                                   swift::Color const& color)
  : Direction(dir)
  , Color(color) {}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightMaterial::draw_quad(RenderContext const& ctx,
                               math::mat3 const& object_transform,
                               float object_depth) {

  DirectionalLightShader::instance()->use(ctx);
  DirectionalLightShader::instance()->projection.Set(math::mat3());
  DirectionalLightShader::instance()->transform.Set(math::mat3());
  DirectionalLightShader::instance()->screen_size.Set(ctx.size);
  DirectionalLightShader::instance()->depth.Set(1.f);
  DirectionalLightShader::instance()->parallax.Set(1.f);
  DirectionalLightShader::instance()->g_buffer_normal.Set(2);
  DirectionalLightShader::instance()->g_buffer_aux_1.Set(3);
  DirectionalLightShader::instance()->light_dir.Set(math::normalized(Direction()));
  DirectionalLightShader::instance()->light_color.Set(Color().vec3());

  Quad::instance()->draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightMaterial::accept(SavableObjectVisitor& visitor) {
  Material::accept(visitor);
  visitor.add_member("Direction", Direction);
  visitor.add_member("Color", Color);
}

////////////////////////////////////////////////////////////////////////////////

}
