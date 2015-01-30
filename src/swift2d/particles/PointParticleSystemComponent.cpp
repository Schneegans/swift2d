////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
  , StartColor(Color(1, 1, 1, 1)), MidColor(Color(1, 1, 1, 0.5)), EndColor(Color(1, 1, 1, 0))
  , BlendAdd(false) {}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.MidLife     = MidLife();
  s.Scale = Scale();
  s.StartGlow = StartGlow();
  s.MidGlow = MidGlow();
  s.EndGlow = EndGlow();
  s.StartColor = StartColor().vec4();
  s.MidColor = MidColor().vec4();
  s.EndColor = EndColor().vec4();
  s.BlendAdd = BlendAdd();

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
  visitor.add_member("StartColor",  StartColor);
  visitor.add_member("MidColor",    MidColor);
  visitor.add_member("EndColor",    EndColor);
  visitor.add_member("BlendAdd",    BlendAdd);
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

      if (o.BlendAdd) {
        ogl::Context::BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::One);
      }

      auto& shader(PointParticleShader::get());
      shader.use(ctx);
      shader.projection. Set(ctx.projection_matrix);
      shader.start_color.Set(o.StartColor);
      shader.mid_color.  Set(o.MidColor);
      shader.end_color.  Set(o.EndColor);
      shader.glow_mid_life.Set(math::vec4(o.StartGlow, o.MidGlow, o.EndGlow, o.MidLife));

      o.System->draw_particles(ctx);

      if (o.BlendAdd) {
        ogl::Context::BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::OneMinusSrcAlpha);
      }
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
