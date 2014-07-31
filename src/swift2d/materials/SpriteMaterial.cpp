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
#include <swift2d/materials/SpriteShaderFactory.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteMaterial::SpriteMaterial()
  : Diffuse(Color(1, 1, 1))
  , Emit(1.0)
  , Glow(0.0)
  , Shinyness(1.0)
  , BlendAdditive(false)
  , current_shader_dirty_(true)
  , current_shader_(nullptr) {

  AnimatedDiffuseTexture.on_change().connect([&](TexturePtr const& val){
    current_shader_dirty_ = true;
  });

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
}

////////////////////////////////////////////////////////////////////////////////

void SpriteMaterial::draw_quad(RenderContext const& ctx, math::mat3 const& object_transform,
               float object_depth) {

  if (current_shader_dirty_) {
    current_shader_dirty_ = false;

    int capabilities(0);

    if (AnimatedDiffuseTexture()) {
      capabilities |= SpriteShaderFactory::ANIMATED_DIFFUSE_TEX;
    }

    if (DiffuseTexture()) {
      capabilities |= SpriteShaderFactory::DIFFUSE_TEX;
    }

    if (AnimatedNormalTexture()) {
      capabilities |= SpriteShaderFactory::ANIMATED_NORMAL_TEX;
    }

    if (NormalTexture()) {
      capabilities |= SpriteShaderFactory::NORMAL_TEX;
    }

    if (AnimatedEmitTexture()) {
      capabilities |= SpriteShaderFactory::ANIMATED_EMIT_TEX;
    }

    if (EmitTexture()) {
      capabilities |= SpriteShaderFactory::EMIT_TEX;
    }

    if (AnimatedGlowTexture()) {
      capabilities |= SpriteShaderFactory::ANIMATED_GLOW_TEX;
    }

    if (GlowTexture()) {
      capabilities |= SpriteShaderFactory::GLOW_TEX;
    }

    if (AnimatedShinynessTexture()) {
      capabilities |= SpriteShaderFactory::ANIMATED_SHINYNESS_TEX;
    }

    if (ShinynessTexture()) {
      capabilities |= SpriteShaderFactory::SHINYNESS_TEX;
    }

    current_shader_ = SpriteShaderFactory::instance()->get_shader(capabilities);

    timer_.start();
  }

  current_shader_->use(ctx);
  current_shader_->projection.Set(ctx.projection_matrix);
  current_shader_->transform.Set(object_transform);
  current_shader_->depth.Set(object_depth);
  current_shader_->parallax.Set(ctx.projection_parallax);

  bool needs_time(false);

  if (AnimatedDiffuseTexture()) {
    AnimatedDiffuseTexture()->bind(ctx, 0);
    current_shader_->diffuse_tex.Set(0);
    needs_time = true;

  } else if (DiffuseTexture()) {
    DiffuseTexture()->bind(ctx, 0);
    current_shader_->diffuse_tex.Set(0);
  }
  current_shader_->diffuse.Set(Diffuse().vec4());


  if (AnimatedNormalTexture()) {
    AnimatedNormalTexture()->bind(ctx, 0);
    current_shader_->normal_tex.Set(0);
    current_shader_->normal_transform.Set(math::make_rotation(math::get_rotation(object_transform)));
    needs_time = true;

  } else if (NormalTexture()) {
    NormalTexture()->bind(ctx, 1);
    current_shader_->normal_tex.Set(1);
    current_shader_->normal_transform.Set(math::make_rotation(math::get_rotation(object_transform)));
  }

  if (AnimatedEmitTexture()) {
    AnimatedEmitTexture()->bind(ctx, 2);
    current_shader_->emit_tex.Set(2);
    needs_time = true;

  } else if (EmitTexture()) {
    EmitTexture()->bind(ctx, 2);
    current_shader_->emit_tex.Set(2);
  }
  current_shader_->emit.Set(Emit());

  if (AnimatedGlowTexture()) {
    AnimatedGlowTexture()->bind(ctx, 3);
    current_shader_->glow_tex.Set(3);
    needs_time = true;

  } else if (GlowTexture()) {
    GlowTexture()->bind(ctx, 3);
    current_shader_->glow_tex.Set(3);
  }
  current_shader_->glow.Set(Glow());

  if (AnimatedShinynessTexture()) {
    AnimatedShinynessTexture()->bind(ctx, 4);
    current_shader_->shinyness_tex.Set(4);
    needs_time = true;

  } else if (ShinynessTexture()) {
    ShinynessTexture()->bind(ctx, 4);
    current_shader_->shinyness_tex.Set(4);
  }
  current_shader_->shinyness.Set(Shinyness());

  if (needs_time) {
    current_shader_->time.Set(timer_.get_elapsed());
  }

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
  visitor.add_object("AnimatedDiffuseTexture", AnimatedDiffuseTexture);
  visitor.add_object("DiffuseTexture", DiffuseTexture);
  visitor.add_member("Diffuse", Diffuse);
  visitor.add_object("NormalTexture", NormalTexture);
  visitor.add_object("EmitTexture", EmitTexture);
  visitor.add_member("Emit", Emit);
  visitor.add_object("GlowTexture", GlowTexture);
  visitor.add_member("Glow", Glow);
  visitor.add_object("ShinynessTexture", ShinynessTexture);
  visitor.add_member("Shinyness", Shinyness);
  visitor.add_member("BlendAdditive", BlendAdditive);
}

////////////////////////////////////////////////////////////////////////////////

}
