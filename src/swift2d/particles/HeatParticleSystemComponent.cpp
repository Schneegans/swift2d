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

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatParticleSystemComponent::HeatParticleSystemComponent()
  : StartScale(1.f),    EndScale(1.f)
  , StartOpacity(1.f),  EndOpacity(0.f) {}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.StartScale = StartScale();
  s.EndScale = EndScale();
  s.StartOpacity = StartOpacity();
  s.EndOpacity = EndOpacity();
  s.Texture = Texture();

  scene->renderers().heat_particle_systems.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void HeatParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("StartScale",    StartScale);
  visitor.add_member("EndScale",      EndScale);
  visitor.add_member("StartOpacity",  StartOpacity);
  visitor.add_member("EndOpacity",    EndOpacity);
  visitor.add_object_property("Texture",       Texture);
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

      o.Texture->bind(ctx, 0);

      auto& shader(HeatParticleShader::get());
      shader.use(ctx);
      shader.projection. Set(ctx.projection_matrix);
      shader.diffuse.    Set(0);
      shader.scale.      Set(math::vec2(o.StartScale, o.EndScale));
      shader.opacity.    Set(math::vec2(o.StartOpacity, o.EndOpacity));

      o.System->draw_particles(ctx);
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
