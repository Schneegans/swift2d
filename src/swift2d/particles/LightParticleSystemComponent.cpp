////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/LightParticleSystemComponent.hpp>

#include <swift2d/particles/LightParticleShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightParticleSystemComponent::LightParticleSystemComponent()
  : StartScale(1.f),               EndScale(1.f)
  , StartColor(Color(1, 1, 1, 1)), EndColor(Color(1, 1, 1, 0)) {}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::draw(RenderContext const& ctx) {
  // SWIFT_PUSH_GL_RANGE("Draw LightParticleSystem");

  // if (ParticleSystemComponent::update_particles(ctx) > 0) {

  //   Texture()->bind(ctx, 0);

  //   auto& shader(LightParticleShader::get());
  //   shader.use(ctx);
  //   shader.projection.       Set(ctx.projection_matrix);
  //   shader.diffuse.          Set(0);
  //   shader.scale.            Set(math::vec2(StartScale(), EndScale()));
  //   shader.start_color.      Set(StartColor().vec4());
  //   shader.end_color.        Set(EndColor().vec4());
  //   shader.screen_size.      Set(ctx.g_buffer_size);
  //   shader.g_buffer_diffuse. Set(1);
  //   shader.g_buffer_normal.  Set(2);
  //   shader.g_buffer_light.   Set(3);

  //   ParticleSystemComponent::draw_particles(ctx);

  // }

  // SWIFT_POP_GL_RANGE();
}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  // ParticleSystemComponent::serialize(scene);
  // scene->lights.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("StartScale",  StartScale);
  visitor.add_member("EndScale",    EndScale);
  visitor.add_member("StartColor",  StartColor);
  visitor.add_member("EndColor",    EndColor);
  visitor.add_object("Texture",     Texture);
}

////////////////////////////////////////////////////////////////////////////////

}
