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
#include <swift2d/graphics/GlowEffect.hpp>

#include <swift2d/graphics/GBuffer.hpp>
#include <swift2d/geometries/Quad.hpp>

#define BLUR_PASSES 6

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GlowEffect::GlowEffect(RenderContext const& ctx)
: blur_shader_(R"(
    // vertex shader ---------------------------------------------------------
    @include "version"

    layout(location=0) in vec2 position;

    uniform sampler2D input_tex;
    uniform vec2 blur_size;

    out vec2 texcoords;
    out vec2 offset;

    void main(void){
      texcoords = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
      offset = blur_size/textureSize(input_tex, 0);
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )", R"(
    // fragment shader -------------------------------------------------------
    @include "version"

    uniform sampler2D input_tex;

    in vec2 texcoords;
    in vec2 offset;

    layout (location = 0) out vec3 fragColor;

    const float curve[7] = float[]( 0.0205,
                                    0.0855,
                                    0.232,
                                    0.324,
                                    0.232,
                                    0.0855,
                                    0.0205 );

    void main(void) {
      vec2 coords = texcoords - offset * 3.0;
      fragColor = vec3(0);
      for(int l=0; l<7; l++) {
        vec3 col   = texture(input_tex, coords).rgb;
        fragColor += col * curve[l];
        coords    += offset;
      }
    }
  )")
  , downsample_shader_(R"(
    // vertex shader ---------------------------------------------------------
    @include "version"

    layout(location=0) in vec2 position;

    uniform sampler2D input_tex;

    out vec2 texcoords[4];

    void main(void){
      vec2 texel_size = 1.0/textureSize(input_tex, 0);
      texcoords[0] = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
      texcoords[1] = texcoords[0] + texel_size * vec2(0.5, 0);
      texcoords[2] = texcoords[0] + texel_size * vec2(0, 0.5);
      texcoords[3] = texcoords[0] + texel_size * vec2(0.5, 0.5);
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )", R"(
    // fragment shader -------------------------------------------------------
    @include "version"

    uniform sampler2D input_tex;

    in vec2 texcoords[4];

    layout (location = 0) out vec3 fragColor;

    void main(void) {
      fragColor =  texture(input_tex, texcoords[0]).rgb*0.25;
      fragColor += texture(input_tex, texcoords[1]).rgb*0.25;
      fragColor += texture(input_tex, texcoords[2]).rgb*0.25;
      fragColor += texture(input_tex, texcoords[3]).rgb*0.25;
    }
  )")
  , hdr_shader_(R"(
    // vertex shader ---------------------------------------------------------
    @include "version"

    layout(location=0) in vec2 position;

    uniform sampler2D input_tex;

    out vec2 texcoords[4];

    void main(void){
      vec2 texel_size = 1.0/textureSize(input_tex, 0);
      texcoords[0] = vec2(position.x + 1.0, 1.0 + position.y) * 0.5;
      texcoords[1] = texcoords[0] + texel_size * vec2(0.5, 0);
      texcoords[2] = texcoords[0] + texel_size * vec2(0, 0.5);
      texcoords[3] = texcoords[0] + texel_size * vec2(0.5, 0.5);
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )", R"(
    // fragment shader -------------------------------------------------------
    @include "version"

    uniform sampler2D input_tex;
    uniform sampler2D emit_tex;

    in vec2 texcoords[4];

    layout (location = 0) out vec3 fragColor;

    void main(void) {
      fragColor =  texture(input_tex, texcoords[0]).rgb*0.25 * (10*texture(emit_tex, texcoords[0]).b);
      fragColor += texture(input_tex, texcoords[1]).rgb*0.25 * (10*texture(emit_tex, texcoords[1]).b);
      fragColor += texture(input_tex, texcoords[2]).rgb*0.25 * (10*texture(emit_tex, texcoords[2]).b);
      fragColor += texture(input_tex, texcoords[3]).rgb*0.25 * (10*texture(emit_tex, texcoords[3]).b);
    }
  )")
  , blur_size_(blur_shader_.get_uniform<math::vec2>("blur_size"))
  , input_tex_(downsample_shader_.get_uniform<int>("input_tex"))
  , blur_tex_(blur_shader_.get_uniform<int>("input_tex"))
  , hdr_tex_(hdr_shader_.get_uniform<int>("input_tex"))
  , emit_tex_(hdr_shader_.get_uniform<int>("emit_tex"))
  , blur_data_(BLUR_PASSES) {

  auto create_texture = [&](
    ogl::Texture& tex, int width, int height,
    ogl::enums::PixelDataInternalFormat i_format,
    ogl::enums::PixelDataFormat         p_format) {

    ogl::Context::Bound(ogl::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, ogl::PixelDataType::Float, nullptr)
      .MaxLevel(0)
      .MinFilter(ogl::TextureMinFilter::Linear)
      .MagFilter(ogl::TextureMagFilter::Linear)
      .WrapS(ogl::TextureWrap::ClampToBorder)
      .WrapT(ogl::TextureWrap::ClampToBorder);
  };

  auto size(ctx.g_buffer_size);

  for (int i(0); i<BLUR_PASSES; ++i) {
    size = size/2;

    create_texture(
      blur_data_[i].buffer_1_, size.x(), size.y(),
      ogl::PixelDataInternalFormat::RGB16F,
      ogl::PixelDataFormat::RGB
    );

    create_texture(
      blur_data_[i].buffer_2_, size.x(), size.y(),
      ogl::PixelDataInternalFormat::RGB16F,
      ogl::PixelDataFormat::RGB
    );

    blur_data_[i].fbo_.Bind(ogl::Framebuffer::Target::Draw);
    ogl::Framebuffer::AttachColorTexture(
      ogl::Framebuffer::Target::Draw, 0, blur_data_[i].buffer_1_, 0
    );
    ogl::Framebuffer::AttachColorTexture(
      ogl::Framebuffer::Target::Draw, 1, blur_data_[i].buffer_2_, 0
    );
  }
}

////////////////////////////////////////////////////////////////////////////////

void GlowEffect::process(RenderContext const& ctx) {

  auto size(ctx.g_buffer_size);
  const float blur_size(4.f);
  const float spread[] = {0.5f, 1.0f, 2.0f, 4.f, 8.f, 16.f};

  for (int i(0); i<BLUR_PASSES; ++i) {
    size = size/2;
    ogl::Context::Viewport(size.x(), size.y());

    blur_data_[i].fbo_.Bind(ogl::Framebuffer::Target::Draw);

    // down sample -------------------------------------------------------------
    ctx.g_buffer->bind_diffuse(4);

    if (i == 0) {
      ctx.g_buffer->bind_light(5);
      hdr_shader_.use();
      hdr_tex_.Set(4);
      emit_tex_.Set(5);
    } else {
      downsample_shader_.use();
      input_tex_.Set(4);
      ogl::Texture::Active(4);
      ogl::Context::Bind(ose::_2D(), blur_data_[i-1].buffer_1_);
    }

    ogl::Context::DrawBuffer(ogl::FramebufferColorAttachment::_0);
    Quad::get().draw();

    // blur horizontally -------------------------------------------------------
    blur_shader_.use();
    blur_tex_.Set(4);
    ogl::Texture::Active(4);
    ogl::Context::Bind(ose::_2D(), blur_data_[i].buffer_1_);
    blur_size_.Set(math::vec2((blur_size * 0.5f + 1)*spread[i], 0.f));
    ogl::Context::DrawBuffer(ogl::FramebufferColorAttachment::_1);
    Quad::get().draw();

    // blur vertically ---------------------------------------------------------
    blur_size_.Set(math::vec2(0.f, (blur_size * 0.5f + 1)*spread[i]));
    ogl::Texture::Active(4);
    ogl::Context::Bind(ose::_2D(), blur_data_[i].buffer_2_);
    ogl::Context::DrawBuffer(ogl::FramebufferColorAttachment::_0);
    Quad::get().draw();
  }
}

////////////////////////////////////////////////////////////////////////////////

int GlowEffect::bind_buffers(int start, RenderContext const& ctx) {
  for (int i(0); i<BLUR_PASSES; ++i) {
    ogl::Texture::Active(start + i);
    ogl::Context::Bind(ose::_2D(), blur_data_[i].buffer_1_);
  }

  return start + BLUR_PASSES;
}

////////////////////////////////////////////////////////////////////////////////

}
