////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/graphics/SubSampler.hpp>

#include <swift2d/geometries/Quad.hpp>
#include <oglplus/bound/framebuffer.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SubSampler::SubSampler(RenderContext const& ctx, int level)
  : shader_(R"(
      // vertex shader -------------------------------------------------------
      @include "fullscreen_quad_vertext_shader"
    )",
    R"(
      // fragment shader -----------------------------------------------------
      @include "version"

      in vec2 texcoords;

      @include "gbuffer_input"

      layout (location = 0) out vec4 fragColor;
      layout (location = 1) out vec4 fragNormal;
      layout (location = 2) out vec4 fragLight;

      void main(void) {
        fragColor  = texture2D(g_buffer_diffuse, texcoords);
        fragNormal = vec4(0.5, 0.5, 0, 0);
        fragLight  = texture2D(g_buffer_light, texcoords) * vec4(1, 1, 1, 0) + vec4(0, 0, 0, 1) * fragColor.a;
      }
  )")
  , level_(level)
  , original_framebuffer_(ctx.gl.Current(ogl::Framebuffer::Target::Draw))
  , g_buffer_(ctx, false, true, level) {

  original_framebuffer_.Bind(ogl::Framebuffer::Target::Draw);
}

////////////////////////////////////////////////////////////////////////////////

void SubSampler::bind(RenderContext const& ctx, bool additive) {

  original_framebuffer_ = ctx.gl.Current(ogl::Framebuffer::Target::Draw);

  g_buffer_.bind_for_drawing(ctx);

  if (additive) {
    ctx.gl.BlendFuncSeparate(ose::SrcAlpha(), ose::One(),
                             ose::One(),      ose::OneMinusSrcAlpha());
  } else {
    ctx.gl.BlendFuncSeparate(ose::SrcAlpha(), ose::OneMinusSrcAlpha(),
                             ose::One(),      ose::OneMinusSrcAlpha());
  }
}

////////////////////////////////////////////////////////////////////////////////

void SubSampler::draw(RenderContext const& ctx, bool additive) {
  original_framebuffer_.Bind(ogl::Framebuffer::Target::Draw);
  ctx.gl.Viewport(ctx.g_buffer_size.x(), ctx.g_buffer_size.y());

  g_buffer_.bind_diffuse(10);
  g_buffer_.bind_light(11);

  shader_.use(ctx);
  shader_.set_uniform("g_buffer_diffuse", 10);
  shader_.set_uniform("g_buffer_light", 11);

  Quad::get().draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

}
