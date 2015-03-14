////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
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
      @include "fullscreen_quad_vertex_shader"
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
        fragColor  = texture(g_buffer_diffuse, texcoords);
        fragColor.rgb = fragColor.rgb / fragColor.a;
        fragNormal = vec4(0.5, 0.5, 0, 0);
        fragLight  = texture(g_buffer_light, texcoords) * vec4(1, 1, 1, 0) + vec4(0, 0, 0, fragColor.a);
      }
  )")
  , level_(level)
  , original_framebuffer_(ogl::Context::Current(ogl::Framebuffer::Target::Draw))
  , buffer_(ctx, level) {

  original_framebuffer_.Bind(ogl::Framebuffer::Target::Draw);
}

////////////////////////////////////////////////////////////////////////////////

void SubSampler::bind(RenderContext const& ctx) {
  original_framebuffer_ = ogl::Context::Current(ogl::Framebuffer::Target::Draw);
  buffer_.bind_for_drawing(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void SubSampler::draw(RenderContext const& ctx) {
  original_framebuffer_.Bind(ogl::Framebuffer::Target::Draw);
  ogl::Context::Viewport(ctx.g_buffer_size.x(), ctx.g_buffer_size.y());

  buffer_.bind_diffuse(10);
  buffer_.bind_light(11);

  shader_.use();
  shader_.set_uniform("g_buffer_diffuse", 10);
  shader_.set_uniform("g_buffer_light", 11);

  Quad::get().draw();
}

////////////////////////////////////////////////////////////////////////////////

}
