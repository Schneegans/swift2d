////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
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
  , StartBurn(0.f),                   MidBurn(0.f),                   EndBurn(0.f)
  , StartColor(Color(1, 1, 1, 1)),    MidColor(Color(1, 1, 1, 0.5)),  EndColor(Color(1, 1, 1, 0))
  , SubSamplingLevel(1) {}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.MidLife     = MidLife();
  s.StartScale  = StartScale();
  s.MidScale    = MidScale();
  s.EndScale    = EndScale();
  s.StartGlow   = StartGlow();
  s.MidGlow     = MidGlow();
  s.EndGlow     = EndGlow();
  s.StartBurn   = StartBurn();
  s.MidBurn     = MidBurn();
  s.EndBurn     = EndBurn();
  s.StartColor  = StartColor().vec4();
  s.MidColor    = MidColor().vec4();
  s.EndColor    = EndColor().vec4();
  s.SubSamplingLevel = SubSamplingLevel();
  s.Texture = Texture();

  scene->renderers().sprite_particle_systems.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void SpriteParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("MidLife",  MidLife);
  visitor.add_member("StartScale",  StartScale);
  visitor.add_member("MidScale",    MidScale);
  visitor.add_member("EndScale",    EndScale);
  visitor.add_member("StartGlow",   StartGlow);
  visitor.add_member("MidGlow",     MidGlow);
  visitor.add_member("EndGlow",     EndGlow);
  visitor.add_member("StartBurn",   StartBurn);
  visitor.add_member("MidBurn",     MidBurn);
  visitor.add_member("EndBurn",     EndBurn);
  visitor.add_member("StartColor",  StartColor);
  visitor.add_member("MidColor",    MidColor);
  visitor.add_member("EndColor",    EndColor);
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

      ogl::Context::BlendFunc(ose::One(), ose::OneMinusSrcAlpha());

      if (o.SubSamplingLevel > 1) {
        ctx.pipeline->get_sub_sampler(o.SubSamplingLevel)->bind(ctx);
      }

      if (o.Texture) {
        o.Texture->bind(ctx, 0);
      } else {
        DefaultTexture::get().bind(0);
      }

      auto& shader(SpriteParticleShader::get());
      shader.use();
      shader.projection.     Set(ctx.projection_matrix);
      shader.diffuse.        Set(0);
      shader.start_mid_scale.Set(math::vec4(o.StartScale.x(), o.StartScale.y(), o.MidScale.x(), o.MidScale.y()));
      shader.end_scale.      Set(math::vec2(o.EndScale.x(), o.EndScale.y()));
      shader.glow_mid_life.  Set(math::vec4(o.StartGlow, o.MidGlow, o.EndGlow, o.MidLife));
      shader.burn.           Set(math::vec3(o.StartBurn, o.MidBurn, o.EndBurn));
      shader.start_color.    Set(o.StartColor);
      shader.mid_color.      Set(o.MidColor);
      shader.end_color.      Set(o.EndColor);

      o.System->draw_particles(ctx);

      SWIFT_POP_GL_RANGE();
      SWIFT_PUSH_GL_RANGE("Blit");

      ogl::Context::BlendFunc(ose::SrcAlpha(), ose::OneMinusSrcAlpha());

      if (o.SubSamplingLevel > 1) {
        ctx.pipeline->get_sub_sampler(o.SubSamplingLevel)->draw(ctx);
      }

      SWIFT_POP_GL_RANGE();
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
