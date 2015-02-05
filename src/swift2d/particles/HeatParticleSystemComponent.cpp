////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/HeatParticleSystemComponent.hpp>

#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/particles/ParticleSystem.hpp>
#include <swift2d/particles/HeatParticleShader.hpp>
#include <swift2d/textures/DefaultTexture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatParticleSystemComponent::HeatParticleSystemComponent()
  : MidLife(0.5f)
  , StartScale(1.f),    MidScale(1.f),    EndScale(1.f)
  , StartOpacity(1.f),  MidOpacity(0.5f), EndOpacity(0.f) {}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.MidLife       = MidLife();
  s.StartScale    = StartScale();
  s.MidScale      = MidScale();
  s.EndScale      = EndScale();
  s.StartOpacity  = StartOpacity();
  s.MidOpacity    = MidOpacity();
  s.EndOpacity    = EndOpacity();
  s.Texture       = Texture();

  scene->renderers().heat_particle_systems.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("MidLife",        MidLife);
  visitor.add_member("StartScale",     StartScale);
  visitor.add_member("MidScale",       MidScale);
  visitor.add_member("EndScale",       EndScale);
  visitor.add_member("StartOpacity",   StartOpacity);
  visitor.add_member("MidOpacity",     MidOpacity);
  visitor.add_member("EndOpacity",     EndOpacity);
  visitor.add_object_property("Texture",  Texture);
}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::Renderer::predraw(RenderContext const& ctx) {
  for (auto& object : objects) {
    SWIFT_PUSH_GL_RANGE("Update HeatParticleSystem");
    object.System->update_particles(object, ctx);
    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    SWIFT_PUSH_GL_RANGE("Draw HeatParticleSystem");

    if (o.System->get_particle_count() > 0) {

      if (o.Texture) {
        o.Texture->bind(ctx, 0);
      } else {
        DefaultTexture::get().bind(ctx, 0);
      }

      auto& shader(HeatParticleShader::get());
      shader.use(ctx);
      shader.projection.    Set(ctx.projection_matrix);
      shader.diffuse.       Set(0);
      shader.scale_mid_life.Set(math::vec4(o.StartScale, o.MidScale, o.EndScale, o.MidLife));
      shader.opacity.       Set(math::vec3(o.StartOpacity, o.MidOpacity, o.EndOpacity));

      o.System->draw_particles(ctx);
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
