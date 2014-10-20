////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/LensFlareEffect.hpp>

#include <swift2d/geometries/Quad.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

LensFlareEffect::LensFlareEffect(RenderContext const& ctx)
: mix_shader_(R"(
    // vertex shader ---------------------------------------------------------
    @include "fullscreen_quad_vertex_shader"
  )", R"(
    // fragment shader -------------------------------------------------------
    @include "version"

    in vec2 texcoords;
	uniform sampler2D inputs[8];

    layout (location = 0) out vec3 fragColor;

    void main(void) {
    vec3 color = vec3(0);

	for (int i=0; i<8; ++i) {
		color += texture2D(inputs[i], texcoords).rgb;
	}

    fragColor = color;
    }
  )")
  , threshold_shader_(R"(
    // vertex shader ---------------------------------------------------------
    @include "version"

    layout(location=0) in vec2 position;
    uniform ivec2 screen_size;

    out vec2 texcoord1;
    out vec2 texcoord2;
    out vec2 texcoord3;
    out vec2 texcoord4;

    void main(void){
    texcoord1 = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
    texcoord2 = texcoord1 + vec2(0.5/screen_size.x, 0.0);
    texcoord3 = texcoord1 + vec2(0.0, 0.5/screen_size.y);
    texcoord4 = texcoord1 + vec2(0.5/screen_size.x, 0.5/screen_size.y);
    gl_Position = vec4(position, 0.0, 1.0);
    }
  )", R"(
    // fragment shader -------------------------------------------------------
    @include "version"

    uniform sampler2D g_buffer_diffuse;
    uniform sampler2D g_buffer_light;
    uniform ivec2 screen_size;

    in vec2 texcoord1;
    in vec2 texcoord2;
    in vec2 texcoord3;
    in vec2 texcoord4;

    layout (location = 0) out vec3 fragColor;

    void main(void) {
    vec3 color = texture2D(g_buffer_diffuse, texcoord1).rgb
                + texture2D(g_buffer_diffuse, texcoord2).rgb
                + texture2D(g_buffer_diffuse, texcoord3).rgb
                + texture2D(g_buffer_diffuse, texcoord4).rgb;

    float glow = texture2D(g_buffer_light, texcoord1).b
                + texture2D(g_buffer_light, texcoord2).b
                + texture2D(g_buffer_light, texcoord3).b
                + texture2D(g_buffer_light, texcoord4).b;

    fragColor = pow(glow * color / 16.0, vec3(2));
    }
  )")
  , inputs_(mix_shader_.get_uniform<int>("inputs"))
  , screen_size_(threshold_shader_.get_uniform<math::vec2i>("screen_size"))
  , g_buffer_diffuse_(threshold_shader_.get_uniform<int>("g_buffer_diffuse"))
  , g_buffer_light_(threshold_shader_.get_uniform<int>("g_buffer_light"))
  , ghosts_(ctx)
  , streaks_(ctx) {
  
  auto create_texture = [&](
    ogl::Texture& tex, int width, int height,
    ogl::enums::PixelDataInternalFormat i_format,
    ogl::enums::PixelDataFormat         p_format) {

    ctx.gl.Bound(ogl::Texture::Target::_2D, tex)
    .Image2D(0, i_format, width, height,
    0, p_format, ogl::PixelDataType::UnsignedByte, nullptr)
    .MaxLevel(0)
    .MinFilter(ogl::TextureMinFilter::Linear)
    .MagFilter(ogl::TextureMagFilter::Linear)
    .WrapS(ogl::TextureWrap::ClampToBorder)
    .WrapT(ogl::TextureWrap::ClampToBorder);
  };

  auto size(ctx.g_buffer_size / 6);

  create_texture(
    buffer_, size.x(), size.y(),
    ogl::PixelDataInternalFormat::RGB8,
    ogl::PixelDataFormat::RGB
  );

  fbo_.Bind(ogl::Framebuffer::Target::Draw);
  ogl::Framebuffer::AttachColorTexture(
    ogl::Framebuffer::Target::Draw, 0, buffer_, 0
  );
}

////////////////////////////////////////////////////////////////////////////////

void LensFlareEffect::process(RenderContext const& ctx) {
  // thresholding
  SWIFT_PUSH_GL_RANGE("Thresholding");
  generate_threshold_buffer(ctx);
  SWIFT_POP_GL_RANGE();

  // streaks
  SWIFT_PUSH_GL_RANGE("Streaks");
  streaks_.process(ctx, buffer_);
  SWIFT_POP_GL_RANGE();

  // ghosts
  SWIFT_PUSH_GL_RANGE("Ghosts");
  ghosts_.process(ctx, buffer_);
  SWIFT_POP_GL_RANGE();

  SWIFT_PUSH_GL_RANGE("Combine");
  int start(4);
  start = streaks_.bind_buffers(start, ctx);
  start = ghosts_.bind_buffers(start, ctx);

  std::vector<int> units = { 4, 5, 6, 7, 8, 9, 10, 11 };
  
  fbo_.Bind(ogl::Framebuffer::Target::Draw);
  
  mix_shader_.use(ctx);
  inputs_.Set(units);

  Quad::get().draw(ctx);

  SWIFT_POP_GL_RANGE();
}

////////////////////////////////////////////////////////////////////////////////

void LensFlareEffect::bind_buffer(int location, RenderContext const& ctx) {
  ogl::Texture::Active(location);
  ctx.gl.Bind(ose::_2D(), buffer_);
}

////////////////////////////////////////////////////////////////////////////////

void LensFlareEffect::generate_threshold_buffer(RenderContext const& ctx) {

  auto size(ctx.g_buffer_size);

  ctx.gl.Viewport(size.x() / 6, size.y() / 6);

  fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  ctx.gl.DrawBuffer(oglplus::FramebufferColorAttachment::_0);

  threshold_shader_.use(ctx);
  g_buffer_diffuse_.Set(0);
  g_buffer_light_.Set(1);
  screen_size_.Set(size / 6);

  Quad::get().draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

}
