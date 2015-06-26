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
#include <swift2d/textures/Texture3D.hpp>

#include <swift2d/textures/DefaultTexture3D.hpp>
#include <oglplus/images/png.hpp>
#include <oglplus/images/newton.hpp>
#include <istream>
#include <streambuf>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Texture3D::Texture3D()
  : Texture()
  , tiles_x_(0)
  , tiles_y_(0) {}

////////////////////////////////////////////////////////////////////////////////

Texture3D::Texture3D(std::string const& file_name, unsigned tiles_x, unsigned tiles_y)
  : Texture(file_name)
  , tiles_x_(tiles_x)
  , tiles_y_(tiles_y) {}

////////////////////////////////////////////////////////////////////////////////

unsigned Texture3D::bind(RenderContext const& ctx, unsigned location) const {

  if (!texture_ && !AsyncLoading()) {
    upload_to(ctx, false);
  }

  if (texture_) {
    texture_->Active(location);
    ogl::Context::Bind(ose::_3D(), *texture_);
  } else {
    upload_to(ctx, false);
    DefaultTexture3D::get().bind(location);
  }

  return location;
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::accept(SavableObjectVisitor& visitor) {
  Texture::accept(visitor);
  visitor.add_member("TilesX", tiles_x_);
  visitor.add_member("TilesY", tiles_y_);
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::set_width(int width) const {
  width_ = width;
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::set_height(int height) const {
  height_ = height;
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::set_channels(int channels) const {
  channels_ = channels;
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::set_data(unsigned char* data) const {
  if (data_) delete data_;
  data_ = data;
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::upload_to(RenderContext const& ctx, bool create_mip_maps) const {

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

    int tile_width  (width_  / tiles_x_);
    int tile_height (height_ / tiles_y_);
    int tile_count  (tiles_x_ * tiles_y_);

    auto internal_format(channels_ > 3 ? ogl::InternalFormat::RGBA : ogl::InternalFormat::RGB);
    auto format(channels_ > 3 ? ogl::Format::RGBA : ogl::Format::RGB);

    texture_ = new ogl::Texture();

    auto tex = ogl::Context::Bound(ose::_3D(), *texture_);
    tex.Image3D(0, internal_format, tile_width, tile_height, tile_count, 0,
                format, ogl::DataType::UnsignedByte, nullptr);
    if (create_mip_maps) {
      tex.MaxLevel(1000);
    } else {
      tex.MaxLevel(0);
    }


    for (int tile_y(0); tile_y<tiles_y_; ++tile_y) {
      for (int tile_x(0); tile_x<tiles_x_; ++tile_x) {

        int tile_number(tile_y*tiles_x_ + tile_x);

        std::vector<unsigned char> data(tile_width*tile_height*channels_);

        int d(0);
        int start(tile_y * tile_height * width_ + tile_width * tile_x);
        int stride(width_);

        while (d<data.size()) {
          int x((d/channels_)%(tile_width));
          int y((d/channels_)/(tile_width));

          int pos(start + x + stride*y);

          for (int c(0); c<channels_; ++c) {
            data[d++] = data_[pos*channels_ + c];
          }
        }

        tex.SubImage3D(
          0, 0, 0, tile_number, tile_width, tile_height, 1,
          format, ogl::DataType::UnsignedByte, &data.front()
        );
      }
    }

    if (create_mip_maps) {
      tex.GenerateMipmap()
         .MinFilter(ose::LinearMipmapLinear());
    } else {
      tex.MinFilter(ose::Linear());
    }

    tex.MagFilter(ose::Linear())
       .WrapS(ose::ClampToEdge())
       .WrapT(ose::ClampToEdge())
       .WrapR(ose::ClampToEdge());

    if (layers_.size() > 0) {
      for (auto& layer: layers_) {
        layer.free();
      }
    } else {
      delete[] data_;
      data_ = nullptr;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

}

