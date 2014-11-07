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
#include <swift2d/particles/ParticleSystem.hpp>
#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LightParticleSystemComponent::LightParticleSystemComponent()
  : StartScale(1.f),               EndScale(1.f)
  , StartColor(Color(1, 1, 1, 1)), EndColor(Color(1, 1, 1, 0)) {}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;

  ParticleSystemComponent::serialize(s);

  s.StartScale = StartScale();
  s.EndScale = EndScale();
  s.StartColor = StartColor().vec4();
  s.EndColor = EndColor().vec4();
  s.Texture = Texture();

  scene->renderers().light_particle_systems.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("StartScale",  StartScale);
  visitor.add_member("EndScale",    EndScale);
  visitor.add_member("StartColor",  StartColor);
  visitor.add_member("EndColor",    EndColor);
  visitor.add_object_property("Texture",     Texture);
}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::Renderer::predraw(RenderContext const& ctx) {
  for (auto& object : objects) {
    SWIFT_PUSH_GL_RANGE("Update LightParticleSystem");
    object.System->update_particles(object, ctx);
    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

void LightParticleSystemComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    SWIFT_PUSH_GL_RANGE("Draw LightParticleSystem");

    if (o.System->get_particle_count() > 0) {

      o.Texture->bind(ctx, 3);

      ctx.gl.BlendFunc(
        oglplus::BlendFunction::One,
        oglplus::BlendFunction::One
      );

      auto& shader(LightParticleShader::get());
      shader.use(ctx);
      shader.projection.       Set(ctx.projection_matrix);
      shader.diffuse.          Set(3);
      shader.scale.            Set(math::vec2(o.StartScale, o.EndScale));
      shader.start_color.      Set(o.StartColor);
      shader.end_color.        Set(o.EndColor);
      shader.screen_size.      Set(ctx.g_buffer_size/(ctx.light_sub_sampling ? 2 : 1));
      shader.g_buffer_normal.  Set(1);
      shader.g_buffer_light.   Set(2);

      o.System->draw_particles(ctx);
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
