////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
  : Texture() {

  TilesX.on_change().connect([&](unsigned){
    needs_update_ = true;
  });
  TilesY.on_change().connect([&](unsigned){
    needs_update_ = true;
  });

}

////////////////////////////////////////////////////////////////////////////////

Texture3D::Texture3D(std::string const& file_name, unsigned tiles_x, unsigned tiles_y)
  : Texture(file_name)
  , TilesX(tiles_x)
  , TilesY(tiles_y) {

  TilesX.on_change().connect([&](unsigned){
    needs_update_ = true;
  });
  TilesY.on_change().connect([&](unsigned){
    needs_update_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::bind(RenderContext const& ctx, unsigned location) const {

  if (texture_) {
    texture_->Active(location);
    ctx.gl.Bind(ose::_3D(), *texture_);
  } else {
    upload_to(ctx);
    DefaultTexture3D::get().bind(ctx, location);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::accept(SavableObjectVisitor& visitor) {
  Texture::accept(visitor);
  visitor.add_member("TilesX", TilesX);
  visitor.add_member("TilesY", TilesY);
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::set_width(int width) const{
  width_ = width;
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::set_height(int height) const{
  height_ = height;
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::set_channels(int channels) const{
  channels_ = channels;
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::set_data(unsigned char* data) const{
  if (data_) delete data_;
  data_ = data;
}

////////////////////////////////////////////////////////////////////////////////

void Texture3D::upload_to(RenderContext const& ctx, bool create_mip_maps) const {

  if (!loading_ && FileName() != "") {
    load_texture_data();
  }

  if (data_ && ctx.upload_budget > 0) {
    --ctx.upload_budget;
    loading_ = false;
    needs_update_ = false;

    if (texture_) {
      delete texture_;
    }

    int tile_width  (width_  / TilesX());
    int tile_height (height_ / TilesY());
    int tile_count  (TilesX() * TilesY());

    auto internal_format(channels_ > 3 ? ogl::InternalFormat::RGBA : ogl::InternalFormat::RGB);
    auto format(channels_ > 3 ? ogl::Format::RGBA : ogl::Format::RGB);

    texture_ = new ogl::Texture();

    auto tex = ctx.gl.Bound(ose::_3D(), *texture_);
    tex.Image3D(0, internal_format, tile_width, tile_height, tile_count, 0,
                format, ogl::DataType::UnsignedByte, nullptr);
    if (create_mip_maps) {
      tex.MaxLevel(1000);
    } else {
      tex.MaxLevel(0);
    }


    for (int tile_y(0); tile_y<TilesY(); ++tile_y) {
      for (int tile_x(0); tile_x<TilesX(); ++tile_x) {

        int tile_number(tile_y*TilesX() + tile_x);

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

    if (FileName() != "") {
      free_texture_data();
    } else {
      delete data_;
      data_ = nullptr;
    }

  } else {
    ++ctx.upload_remaining;
  }
}

////////////////////////////////////////////////////////////////////////////////

}

