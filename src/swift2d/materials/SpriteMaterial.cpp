////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/SpriteMaterial.hpp>

#include <swift2d/geometries/Quad.hpp>
#include <swift2d/materials/SpriteShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteMaterial::SpriteMaterial()
  : Diffuse(Color(1, 1, 1))
  , Emit(1.0)
  , Glow(0.0)
  , Shinyness(1.0)
  , Reflectivity(1.0)
  , current_shader_dirty_(true)
  , current_shader_(nullptr) {

  DiffuseTexture.on_change().connect([&](TexturePtr const& val){
    current_shader_dirty_ = true;
  });

  NormalTexture.on_change().connect([&](TexturePtr const& val){
    current_shader_dirty_ = true;
  });

  EmitTexture.on_change().connect([&](TexturePtr const& val){
    current_shader_dirty_ = true;
  });

  GlowTexture.on_change().connect([&](TexturePtr const& val){
    current_shader_dirty_ = true;
  });

  ShinynessTexture.on_change().connect([&](TexturePtr const& val){
    current_shader_dirty_ = true;
  });

  ReflectivityTexture.on_change().connect([&](TexturePtr const& val){
    current_shader_dirty_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void SpriteMaterial::draw_quad(RenderContext const& ctx, math::mat3 const& object_transform,
               float object_depth) {

  if (current_shader_dirty_) {
    current_shader_dirty_ = false;

    int capabilities(0);

    if (DiffuseTexture()) {
      capabilities |= SpriteShader::DIFFUSE_TEX;
    }

    if (NormalTexture()) {
      capabilities |= SpriteShader::NORMAL_TEX;
    }

    if (EmitTexture()) {
      capabilities |= SpriteShader::EMIT_TEX;
    }

    if (GlowTexture()) {
      capabilities |= SpriteShader::GLOW_TEX;
    }

    if (ShinynessTexture()) {
      capabilities |= SpriteShader::SHINYNESS_TEX;
    }

    if (ReflectivityTexture()) {
      capabilities |= SpriteShader::REFLECTIVITY_TEX;
    }

    current_shader_ = SpriteShader::instance()->get_shader(capabilities);
  }

  current_shader_->use(ctx);
  current_shader_->set_uniform("projection", ctx.projection_matrix);
  current_shader_->set_uniform("transform",  object_transform);
  current_shader_->set_uniform("depth",      object_depth);
  current_shader_->set_uniform("parallax",   ctx.projection_parallax);

  if (DiffuseTexture()) {
    DiffuseTexture()->bind(ctx, 0);
    current_shader_->set_uniform("diffuse_tex", 0);
  }
  current_shader_->set_uniform("diffuse", Diffuse().vec3());


  if (NormalTexture()) {
    NormalTexture()->bind(ctx, 1);
    current_shader_->set_uniform("normal_tex", 1);
    current_shader_->set_uniform("normal_transform",  math::transposed(math::inversed(object_transform)));
  }

  if (EmitTexture()) {
    EmitTexture()->bind(ctx, 2);
    current_shader_->set_uniform("emit_tex", 2);
  }
  current_shader_->set_uniform("emit", Emit());

  if (GlowTexture()) {
    GlowTexture()->bind(ctx, 3);
    current_shader_->set_uniform("glow_tex", 3);
  }
  current_shader_->set_uniform("glow", Glow());

  if (ShinynessTexture()) {
    ShinynessTexture()->bind(ctx, 4);
    current_shader_->set_uniform("shinyness_tex", 4);
  }
  current_shader_->set_uniform("shinyness", Shinyness());

  if (ReflectivityTexture()) {
    ReflectivityTexture()->bind(ctx, 5);
    current_shader_->set_uniform("reflectivity_tex", 5);
  }
  current_shader_->set_uniform("reflectivity", Reflectivity());

  if (BlendAdditive()) {
    ctx.gl.BlendFunc(oglplus::BlendFn::SrcAlpha, oglplus::BlendFn::One);
    Quad::instance()->draw(ctx);
    ctx.gl.BlendFunc(oglplus::BlendFn::SrcAlpha, oglplus::BlendFn::OneMinusSrcAlpha);
  } else {
    Quad::instance()->draw(ctx);
  }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteMaterial::accept(SavableObjectVisitor& visitor) {
  Material::accept(visitor);
  visitor.add_object("DiffuseTexture", DiffuseTexture);
  visitor.add_member("Diffuse", Diffuse);
  visitor.add_object("NormalTexture", NormalTexture);
  visitor.add_object("EmitTexture", EmitTexture);
  visitor.add_member("Emit", Emit);
  visitor.add_object("GlowTexture", GlowTexture);
  visitor.add_member("Glow", Glow);
  visitor.add_object("ShinynessTexture", ShinynessTexture);
  visitor.add_member("Shinyness", Shinyness);
  visitor.add_object("ReflectivityTexture", ReflectivityTexture);
  visitor.add_member("Reflectivity", Reflectivity);
  visitor.add_member("BlendAdditive", BlendAdditive);
}

////////////////////////////////////////////////////////////////////////////////

}
