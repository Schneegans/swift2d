////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/GravityMap.hpp>

#include <swift2d/physics/Physics.hpp>
#include <swift2d/physics/GravitySourceComponent.hpp>
#include <swift2d/components/CameraComponent.hpp>
#include <swift2d/geometries/Quad.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

GravityMap::GravityMap(RenderContext const& ctx)
  : gravity_shader_(R"(
      // vertex shader -------------------------------------------------------
      @include "fullscreen_quad_vertex_shader"
    )",
    R"(
      // fragment shader -----------------------------------------------------
      @include "version"

      uniform vec4  gravity_sources[20];
      uniform int   gravity_source_count;
      uniform vec2  world_gravity;
      uniform ivec2 screen_size;

      in vec2 texcoords;

      layout (location = 0) out vec3 fragColor;

      void main(void){

        fragColor = vec3(0.5 + world_gravity*0.1, 0);

        vec2 pos = (texcoords - vec2(0.5))*2.0;

        for (int i=0; i<gravity_source_count; i += 1) {
          vec2 dir = (gravity_sources[i].xy - pos);
          dir.x = dir.x * screen_size.x/screen_size.y;
          float dist = dir.x*dir.x + dir.y*dir.y;
          dir = dir / dist / dist * gravity_sources[i].z;

          fragColor += vec3(dir*0.0001, dist < gravity_sources[i].w);
        }
      }
  )")
  , gravity_sources_(gravity_shader_.get_uniform<math::vec4>("gravity_sources"))
  , screen_size_(gravity_shader_.get_uniform<math::vec2i>("screen_size"))
  , gravity_source_count_(gravity_shader_.get_uniform<int>("gravity_source_count"))
  , world_gravity_(gravity_shader_.get_uniform<math::vec2>("world_gravity")) {

  auto create_texture = [&](
    oglplus::Texture& tex, int width, int height,
    oglplus::enums::PixelDataInternalFormat i_format,
    oglplus::enums::PixelDataFormat         p_format) {

    ogl::Context::Bound(oglplus::Texture::Target::_2D, tex)
      .Image2D(0, i_format, width, height,
        0, p_format, oglplus::PixelDataType::Float, nullptr)
      .MaxLevel(0)
      .MinFilter(oglplus::TextureMinFilter::Linear)
      .MagFilter(oglplus::TextureMagFilter::Linear)
      .WrapS(oglplus::TextureWrap::MirroredRepeat)
      .WrapT(oglplus::TextureWrap::MirroredRepeat);
  };

  create_texture(
    gravity_map_, ctx.window_size.x()/16, ctx.window_size.y()/16,
    oglplus::PixelDataInternalFormat::RGB,
    oglplus::PixelDataFormat::RGB
  );

  gravity_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  oglplus::Framebuffer::AttachColorTexture(
    oglplus::Framebuffer::Target::Draw, 0, gravity_map_, 0
  );
}

////////////////////////////////////////////////////////////////////////////////

void GravityMap::process(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  ogl::Context::Disable(oglplus::Capability::Blend);
  gravity_fbo_.Bind(oglplus::Framebuffer::Target::Draw);
  ogl::Context::Viewport(ctx.window_size.x()/16, ctx.window_size.y()/16);

  std::vector<math::vec4> sources;

  for (auto const& source: scene->gravity_sources) {
    auto pos(math::get_translation(source->WorldTransform()));
    pos = (ctx.projection_matrix * math::vec3(pos.x(), pos.y(), 1.0)).xy();

    auto scale(math::get_scale(source->WorldTransform()));
    float radius = scene->camera->world_to_pixel(math::vec2(0.f, 2*scale.y()*source->CollisionRadius()), false).y() / ctx.window_size.y();
    sources.push_back(math::vec4(pos.x(), pos.y(), source->Mass(), radius*radius));
  }

  gravity_shader_.use(ctx);
  screen_size_.Set(ctx.window_size / 16);
  gravity_source_count_.Set((int)sources.size());
  world_gravity_.Set(Physics::get().Gravity());

  if (sources.size() > 0) {
    gravity_sources_.Set(sources);
  }

  Quad::get().draw(ctx);

  ogl::Context::Enable(oglplus::Capability::Blend);
}

////////////////////////////////////////////////////////////////////////////////

void GravityMap::bind(RenderContext const& ctx, int location) {
  oglplus::Texture::Active(location);
  gravity_map_.Bind(oglplus::Texture::Target::_2D);
}

////////////////////////////////////////////////////////////////////////////////

}
