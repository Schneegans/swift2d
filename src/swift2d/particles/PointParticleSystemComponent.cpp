////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/PointParticleSystemComponent.hpp>

#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/particles/ParticleSystem.hpp>
#include <swift2d/particles/PointParticleShader.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointParticleSystemComponent::PointParticleSystemComponent()
  : MidLife(0.5f)
  , Scale(1.f)
  , StartGlow(0.f),                MidGlow(0.f),  EndGlow(0.f)
  , StartBurn(0.f),                MidBurn(0.f),  EndBurn(0.f)
  , StartColor(Color(1, 1, 1, 1)), MidColor(Color(1, 1, 1, 0.5)), EndColor(Color(1, 1, 1, 0)) {}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.MidLife     = MidLife();
  s.Scale       = Scale();
  s.StartGlow   = StartGlow();
  s.MidGlow     = MidGlow();
  s.EndGlow     = EndGlow();
  s.StartBurn   = StartBurn();
  s.MidBurn     = MidBurn();
  s.EndBurn     = EndBurn();
  s.StartColor  = StartColor().vec4();
  s.MidColor    = MidColor().vec4();
  s.EndColor    = EndColor().vec4();

  scene->renderers().point_particle_systems.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("MidLife",     MidLife);
  visitor.add_member("Scale",       Scale);
  visitor.add_member("StartGlow",   StartGlow);
  visitor.add_member("MidGlow",     MidGlow);
  visitor.add_member("EndGlow",     EndGlow);
  visitor.add_member("StartBurn",   StartBurn);
  visitor.add_member("MidBurn",     MidBurn);
  visitor.add_member("EndBurn",     EndBurn);
  visitor.add_member("StartColor",  StartColor);
  visitor.add_member("MidColor",    MidColor);
  visitor.add_member("EndColor",    EndColor);
}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::Renderer::predraw(RenderContext const& ctx) {
  for (auto& object : objects) {
    SWIFT_PUSH_GL_RANGE("Update PointParticleSystem");
    object.System->update_particles(object, ctx);
    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    SWIFT_PUSH_GL_RANGE("Draw PointParticleSystem");

    if (o.System->get_particle_count() > 0) {

      ogl::Context::Rasterization::PointSize(o.Scale);

      ogl::Context::BlendFunc(ose::One(), ose::OneMinusSrcAlpha());

      auto& shader(PointParticleShader::get());
      shader.use(ctx);
      shader.projection. Set(ctx.projection_matrix);
      shader.start_color.Set(o.StartColor);
      shader.mid_color.  Set(o.MidColor);
      shader.end_color.  Set(o.EndColor);
      shader.glow_mid_life.Set(math::vec4(o.StartGlow, o.MidGlow, o.EndGlow, o.MidLife));
      shader.burn.       Set(math::vec3(o.StartBurn, o.MidBurn, o.EndBurn));

      o.System->draw_particles(ctx);

      ogl::Context::BlendFunc(ose::SrcAlpha(), ose::OneMinusSrcAlpha());
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
