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
#include <swift2d/application/Paths.hpp>
#include <oglplus/images/png.hpp>
#include <oglplus/images/newton.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

#include <thread>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Texture::Layer::Layer(std::string const& f, Color const& c)
  : FileName(f)
  , Colorize(c)
  , data_(nullptr)
  , width_(0)
  , height_(0)
  , channels_(0) {}

////////////////////////////////////////////////////////////////////////////////

void Texture::Layer::load() {
  int w(0), h(0), c(0);
  auto d(stbi_load(Paths::get().make_absolute(FileName).c_str(), &w, &h, &c, STBI_default));
  if (d) {
    data_ = d;
    width_ = w;
    height_ = h;
    channels_ = c;

    for (int i(0); i<width_*height_*channels_; i+=channels_) {
      for (int c(0); c<channels_; ++c) {
        data_[i+c] = data_[i+c] * Colorize[c];
      }
    }
  } else {
    LOG_ERROR << "Failed to load texture \"" << FileName << "\"!" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Texture::Layer::free() {
  if (data_) {
    stbi_image_free(data_);
    data_ = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Texture::Layer::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("FileName", FileName);
  visitor.add_member("Colorize", Colorize);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Texture::Texture()
  : AsyncLoading(true)
  , Repeat(true)
  , texture_(nullptr)
  , loading_(false)
  , data_(nullptr)
  , width_(0)
  , height_(0)
  , channels_(0) {}

////////////////////////////////////////////////////////////////////////////////

Texture::Texture(std::string const& file_name)
  : AsyncLoading(true)
  , Repeat(true)
  , texture_(nullptr)
  , loading_(false)
  , data_(nullptr)
  , width_(0)
  , height_(0)
  , channels_(0) {

  layers_.push_back(Layer(file_name));
}

////////////////////////////////////////////////////////////////////////////////

Texture::Texture(std::vector<Layer> const& layers)
  : AsyncLoading(true)
  , Repeat(true)
  , texture_(nullptr)
  , loading_(false)
  , data_(nullptr)
  , width_(0)
  , height_(0)
  , channels_()
  , layers_(layers) {}

////////////////////////////////////////////////////////////////////////////////

Texture::~Texture() {
  if (texture_) delete texture_;
}

////////////////////////////////////////////////////////////////////////////////

void Texture::bind(RenderContext const& ctx, unsigned location) const {

  if (!texture_ && !AsyncLoading()) {
    upload_to(ctx);
  }

  if (texture_) {
    texture_->Active(location);
    ctx.gl.Bind(ose::_2D(), *texture_);
  } else {
    upload_to(ctx);
    DefaultTexture::get().bind(ctx, location);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Texture::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("AsyncLoading", AsyncLoading);
  visitor.add_member("Repeat", Repeat);
  visitor.add_array("Layers", layers_);
}

////////////////////////////////////////////////////////////////////////////////

void Texture::upload_to(RenderContext const& ctx) const {

  if (layers_.size() > 0) {
    if (!data_) {
      if (!loading_) {
        if (ctx.upload_budget > 0) {
          --ctx.upload_budget;
          load_texture_data();
        } else if (!AsyncLoading()) {
          load_texture_data();
        } else {
          ++ctx.upload_remaining;
        }
      }

    } else {

      loading_ = false;

      if (texture_) {
        delete texture_;
      }

      auto internal_format(channels_ > 3 ? ogl::InternalFormat::RGBA : ogl::InternalFormat::RGB);
      auto format(channels_ > 3 ? ogl::Format::RGBA : ogl::Format::RGB);

      texture_ = new ogl::Texture();

      ctx.gl.Bound(ose::_2D(), *texture_)
        .Image2D(0, internal_format, width_, height_, 0, format,
                 ogl::DataType::UnsignedByte, data_)
        .MaxLevel(1000)
        .GenerateMipmap()
        .MinFilter(ose::LinearMipmapLinear())
        .MagFilter(ose::Linear())
        .WrapS(Repeat() ? oglplus::TextureWrap::Repeat : oglplus::TextureWrap::ClampToEdge)
        .WrapT(Repeat() ? oglplus::TextureWrap::Repeat : oglplus::TextureWrap::ClampToEdge);

      for (auto& layer: layers_) {
        layer.free();
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void Texture::load_texture_data() const {
  loading_ = true;

  auto load = [this](){
    for (auto& layer: layers_) {
      layer.load();
    }
    data_ = layers_[0].data_;
    width_ = layers_[0].width_;
    height_ = layers_[0].height_;
    channels_ = layers_[0].channels_;

    for (int i(1); i<layers_.size(); ++i) {
      if (layers_[i].width_ == width_ &&
          layers_[i].height_ == height_ &&
          layers_[i].channels_ == channels_) {

        for (int d(0); d<width_*height_*channels_; d+=channels_) {
          if (channels_ == 4) {
            unsigned char a(layers_[i].data_[d+3]);
            data_[d+0] = (data_[d+0] * (255-a) + layers_[i].data_[d+0] * a)/255;
            data_[d+1] = (data_[d+1] * (255-a) + layers_[i].data_[d+1] * a)/255;
            data_[d+2] = (data_[d+2] * (255-a) + layers_[i].data_[d+2] * a)/255;
            data_[d+3] = (data_[d+3] *  255    + (255-data_[d+3])      * a)/255;
          } else {
            for (int c(0); c<channels_; ++c) {
              data_[d+c] = std::min(255, data_[d+c] + layers_[i].data_[d+c]);
            }
          }
        }

      } else {
        LOG_WARNING << "Failed to add texture layer " << layers_[i].FileName
                    << ": Dimensions do not match!" << std::endl;
      }
    }
  };

  if (AsyncLoading()) {
    std::thread loading_thread(load);
    loading_thread.detach();
  } else {
    load();
  }
}

////////////////////////////////////////////////////////////////////////////////

}

