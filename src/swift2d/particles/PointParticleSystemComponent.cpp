////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/PointParticleSystemComponent.hpp>

#include <swift2d/particles/PointParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

PointParticleSystemComponent::PointParticleSystemComponent()
  : Scale(1.f)
  , StartGlow(0.f),                EndGlow(0.f)
  , StartColor(Color(1, 1, 1, 1)), EndColor(Color(1, 1, 1, 0))
  , BlendAdd(false) {}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::draw(RenderContext const& ctx) {

  SWIFT_PUSH_GL_RANGE("Draw PointParticleSystem");

  if (ParticleSystemComponent::update_particles(ctx) > 0) {

    ctx.gl.Rasterization::PointSize(Scale());

    if (BlendAdd()) {
      ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::One);
    }

    auto& shader(PointParticleShader::get());
    shader.use(ctx);
    shader.projection. Set(ctx.projection_matrix);
    shader.start_color.Set(StartColor().vec4());
    shader.end_color.  Set(EndColor().vec4());
    shader.glow.       Set(math::vec2(StartGlow(), EndGlow()));

    ParticleSystemComponent::draw_particles(ctx);

    if (BlendAdd()) {
      ctx.gl.BlendFunc(ogl::BlendFunction::SrcAlpha, ogl::BlendFunction::OneMinusSrcAlpha);
    }

  }

  SWIFT_POP_GL_RANGE();
}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  ParticleSystemComponent::serialize(scene);
  scene->objects[Depth.get()].add_object(create_copy());
}

////////////////////////////////////////////////////////////////////////////////

void PointParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("Scale",       Scale);
  visitor.add_member("StartGlow",   StartGlow);
  visitor.add_member("EndGlow",     EndGlow);
  visitor.add_member("StartColor",  StartColor);
  visitor.add_member("EndColor",    EndColor);
  visitor.add_member("BlendAdd",    BlendAdd);
}

////////////////////////////////////////////////////////////////////////////////

}
