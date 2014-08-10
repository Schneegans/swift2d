////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/Texture.hpp>

#include <swift2d/textures/DefaultTexture.hpp>
#include <swift2d/Swift2D.hpp>
#include <oglplus/images/png.hpp>
#include <oglplus/images/newton.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

#include <thread>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Texture::Texture()
  : FileName("")
  , texture_(nullptr)
  , needs_update_(true)
  , loading_(false)
  , data_(nullptr)
  , width_(0)
  , height_(0)
  , channels_(0) {


  FileName.on_change().connect([&](std::string const& path){
    needs_update_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

Texture::Texture(std::string const& file_name)
  : FileName(file_name)
  , texture_(nullptr)
  , needs_update_(true)
  , loading_(false)
  , data_(nullptr)
  , width_(0)
  , height_(0)
  , channels_(0) {

  FileName.on_change().connect([&](std::string const& path){
    needs_update_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

Texture::~Texture() {
  if (texture_) delete texture_;
}

////////////////////////////////////////////////////////////////////////////////

void Texture::bind(RenderContext const& ctx, unsigned location) const {

  if (texture_) {
    texture_->Active(location);
    ctx.gl.Bind(ose::_2D(), *texture_);
  } else {
    upload_to(ctx);
    DefaultTexture::instance()->bind(ctx, location);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Texture::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("FileName", FileName);
}

////////////////////////////////////////////////////////////////////////////////

void Texture::upload_to(RenderContext const& ctx) const {

  if (!loading_) {
    load_texture_data();
  }

  if (data_ && ctx.upload_budget > 0) {
    --ctx.upload_budget;
    loading_ = false;
    needs_update_ = false;

    if (texture_) {
      delete texture_;
    }


    auto internal_format(channels_ > 3 ? ogl::InternalFormat::RGBA : ogl::InternalFormat::RGB);
    auto format(channels_ > 3 ? ogl::Format::RGBA : ogl::Format::RGB);

    texture_ = new ogl::Texture();

    ctx.gl.Bound(ose::_2D(), *texture_)
      .Image2D(0, internal_format, width_, height_, 0, format,
               ogl::DataType::UnsignedByte, data_)
      .GenerateMipmap()
      .MinFilter(ose::LinearMipmapLinear())
      .MagFilter(ose::Linear())
      .WrapS(ose::Repeat())
      .WrapT(ose::Repeat());

    free_texture_data();
  } else {
    ++ctx.upload_remaining;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Texture::load_texture_data() const {

  std::string f(FileName());

  if (f.length() > 0) {
    loading_ = true;

    if (f[0] != '/') {
      f = Swift2D::instance()->make_absolute(f);
    }

    std::thread load([this, f](){
      int w(0), h(0), c(0);
      auto d(stbi_load(f.c_str(), &w, &h, &c, STBI_default));

      if (d) {
        data_ = d;
        width_ = w;
        height_ = h;
        channels_ = c;
      } else {
        Logger::LOG_ERROR << "Failed to load texture \"" << FileName() << "\"!" << std::endl;
      }
    });

    load.detach();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Texture::free_texture_data() const {
  stbi_image_free(data_);
  data_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

}

