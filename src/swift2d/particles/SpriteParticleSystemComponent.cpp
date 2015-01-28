////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/SpriteParticleSystemComponent.hpp>
#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/particles/SpriteParticleShader.hpp>
#include <swift2d/particles/ParticleSystem.hpp>
#include <swift2d/graphics/Pipeline.hpp>
#include <swift2d/textures/DefaultTexture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteParticleSystemComponent::SpriteParticleSystemComponent()
  : MidLife(0.5f)
  , StartScale(math::vec2(1.f, 1.f)), MidScale(math::vec2(1.f, 1.f)), EndScale(math::vec2(1.f, 1.f))
  , StartGlow(0.f),                   MidGlow(0.f),                   EndGlow(0.f)
  , StartColor(Color(1, 1, 1, 1)),    MidColor(Color(1, 1, 1, 0.5)),  EndColor(Color(1, 1, 1, 0))
  , BlendAdd(false)
  , SubSamplingLevel(1) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.MidLife     = MidLife();
  s.StartScale  = StartScale();
  s.StartGlow   = StartGlow();
  s.StartColor  = StartColor().vec4();
  s.MidScale    = MidScale();
  s.MidGlow     = MidGlow();
  s.MidColor    = MidColor().vec4();
  s.EndScale    = EndScale();
  s.EndGlow     = EndGlow();
  s.EndColor    = EndColor().vec4();
  s.BlendAdd    = BlendAdd();
  s.SubSamplingLevel = SubSamplingLevel();
  s.Texture = Texture();

  scene->renderers().sprite_particle_systems.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("MidLife",  MidLife);
  visitor.add_member("StartScale",  StartScale);
  visitor.add_member("StartGlow",   StartGlow);
  visitor.add_member("StartColor",  StartColor);
  visitor.add_member("MidScale",    MidScale);
  visitor.add_member("MidGlow",     MidGlow);
  visitor.add_member("MidColor",    MidColor);
  visitor.add_member("EndScale",    EndScale);
  visitor.add_member("EndGlow",     EndGlow);
  visitor.add_member("EndColor",    EndColor);
  visitor.add_member("BlendAdd",    BlendAdd);
  visitor.add_member("SubSamplingLevel", SubSamplingLevel);
  visitor.add_object_property("Texture",     Texture);
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::Renderer::predraw(RenderContext const& ctx) {
  for (auto& object : objects) {
    SWIFT_PUSH_GL_RANGE("Update SpriteParticleSystem");
    object.System->update_particles(object, ctx);
    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    SWIFT_PUSH_GL_RANGE("Draw SpriteParticleSystem");

    if (o.System->get_particle_count() > 0) {

      SWIFT_PUSH_GL_RANGE("Draw");

      if (o.SubSamplingLevel > 1) {
        ctx.pipeline->get_sub_sampler(o.SubSamplingLevel)->bind(ctx, o.BlendAdd);
      } else if (o.BlendAdd) {
        ogl::Context::BlendFunc(ose::SrcAlpha(), ose::One());
      }

      if (o.Texture) {
        o.Texture->bind(ctx, 0);
      } else {
        DefaultTexture::get().bind(ctx, 0);
      }

      auto& shader(SpriteParticleShader::get());
      shader.use(ctx);
      shader.projection.     Set(ctx.projection_matrix);
      shader.diffuse.        Set(0);
      shader.start_mid_scale.Set(math::vec4(o.StartScale.x(), o.StartScale.y(), o.MidScale.x(), o.MidScale.y()));
      shader.end_scale.      Set(math::vec2(o.EndScale.x(), o.EndScale.y()));
      shader.glow_mid_life.  Set(math::vec4(o.StartGlow, o.MidGlow, o.EndGlow, o.MidLife));
      shader.start_color.    Set(o.StartColor);
      shader.mid_color.      Set(o.MidColor);
      shader.end_color.      Set(o.EndColor);

      o.System->draw_particles(ctx);

      SWIFT_POP_GL_RANGE();
      SWIFT_PUSH_GL_RANGE("Blit");

      if (o.SubSamplingLevel > 1) {
        ctx.pipeline->get_sub_sampler(o.SubSamplingLevel)->draw(ctx, o.BlendAdd);
      } else if (o.BlendAdd) {
        ogl::Context::BlendFunc(ose::SrcAlpha(), ose::OneMinusSrcAlpha());
      }

      SWIFT_POP_GL_RANGE();
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
