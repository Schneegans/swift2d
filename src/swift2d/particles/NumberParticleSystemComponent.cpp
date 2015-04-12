////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/particles/NumberParticleSystemComponent.hpp>
#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/particles/NumberParticleShader.hpp>
#include <swift2d/particles/ParticleSystem.hpp>
#include <swift2d/graphics/Pipeline.hpp>
#include <swift2d/textures/DefaultTexture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

NumberParticleSystemComponent::NumberParticleSystemComponent()
  : MidLife(0.5f)
  , StartScale(1.f), MidScale(1.f), EndScale(1.f)
  , StartGlow(0.f),  MidGlow(0.f),  EndGlow(0.f)
  , StartBurn(0.f),  MidBurn(0.f),  EndBurn(0.f)
  , ScaleBoost(1.f)
  , LowerBoostLimit(0), UpperBoostLimit(100)
  , PositiveStartColor(Color(1, 1, 1, 1)),    PositiveMidColor(Color(1, 1, 1, 0.5)),  PositiveEndColor(Color(1, 1, 1, 0))
  , NegativeStartColor(Color(1, 1, 1, 1)),    NegativeMidColor(Color(1, 1, 1, 0.5)),  NegativeEndColor(Color(1, 1, 1, 0)) {

  RotateToSpawnDirection = 0.f;
}

////////////////////////////////////////////////////////////////////////////////

void NumberParticleSystemComponent::serialize(SerializedScenePtr& scene) const {
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
  s.ScaleBoost  = ScaleBoost();
  s.LowerBoostLimit     = LowerBoostLimit();
  s.UpperBoostLimit     = UpperBoostLimit();
  s.PositiveStartColor  = PositiveStartColor().vec4();
  s.PositiveMidColor    = PositiveMidColor().vec4();
  s.PositiveEndColor    = PositiveEndColor().vec4();
  s.NegativeStartColor  = NegativeStartColor().vec4();
  s.NegativeMidColor    = NegativeMidColor().vec4();
  s.NegativeEndColor    = NegativeEndColor().vec4();
  s.Font                = Font();

  scene->renderers().number_particle_systems.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void NumberParticleSystemComponent::accept(SavableObjectVisitor& visitor) {
  ParticleSystemComponent::accept(visitor);
  visitor.add_member("MidLife",     MidLife);
  visitor.add_member("StartScale",  StartScale);
  visitor.add_member("MidScale",    MidScale);
  visitor.add_member("EndScale",    EndScale);
  visitor.add_member("StartGlow",   StartGlow);
  visitor.add_member("MidGlow",     MidGlow);
  visitor.add_member("EndGlow",     EndGlow);
  visitor.add_member("StartBurn",   StartBurn);
  visitor.add_member("MidBurn",     MidBurn);
  visitor.add_member("EndBurn",     EndBurn);
  visitor.add_member("ScaleBoost",  ScaleBoost);
  visitor.add_member("LowerBoostLimit",     LowerBoostLimit);
  visitor.add_member("UpperBoostLimit",     UpperBoostLimit);
  visitor.add_member("PositiveStartColor",  PositiveStartColor);
  visitor.add_member("PositiveMidColor",    PositiveMidColor);
  visitor.add_member("PositiveEndColor",    PositiveEndColor);
  visitor.add_member("NegativeStartColor",  NegativeStartColor);
  visitor.add_member("NegativeMidColor",    NegativeMidColor);
  visitor.add_member("NegativeEndColor",    NegativeEndColor);
  visitor.add_object_property("Font",       Font);
}

////////////////////////////////////////////////////////////////////////////////

void NumberParticleSystemComponent::Renderer::predraw(RenderContext const& ctx) {
  for (auto& object : objects) {
    SWIFT_PUSH_GL_RANGE("Update NumberParticleSystem");
    object.System->update_particles(object, ctx);
    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

void NumberParticleSystemComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  for (int i(start); i<end; ++i) {
    auto& o(objects[i]);

    SWIFT_PUSH_GL_RANGE("Draw NumberParticleSystem");

    if (o.System->get_particle_count() > 0) {

      SWIFT_PUSH_GL_RANGE("Draw");

      ogl::Context::BlendFunc(ose::One(), ose::OneMinusSrcAlpha());

      if (o.Font) {
        o.Font->bind(ctx, 0);
      } else {
        DefaultTexture::get().bind(0);
      }

      auto& shader(NumberParticleShader::get());
      shader.use();
      shader.projection.     Set(ctx.projection_matrix);
      shader.font.           Set(0);
      shader.boost_limits.   Set(math::vec2(o.LowerBoostLimit, o.UpperBoostLimit));
      shader.scale_boost.    Set(math::vec4(o.StartScale, o.MidScale, o.EndScale, o.ScaleBoost));
      shader.glow_mid_life.  Set(math::vec4(o.StartGlow,  o.MidGlow,  o.EndGlow, o.MidLife));
      shader.burn.           Set(math::vec3(o.StartBurn,  o.MidBurn,  o.EndBurn));
      shader.p_start_color.  Set(o.PositiveStartColor);
      shader.p_mid_color.    Set(o.PositiveMidColor);
      shader.p_end_color.    Set(o.PositiveEndColor);
      shader.n_start_color.  Set(o.NegativeStartColor);
      shader.n_mid_color.    Set(o.NegativeMidColor);
      shader.n_end_color.    Set(o.NegativeEndColor);

      o.System->draw_particles(ctx);

      SWIFT_POP_GL_RANGE();

      ogl::Context::BlendFunc(ose::SrcAlpha(), ose::OneMinusSrcAlpha());
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
