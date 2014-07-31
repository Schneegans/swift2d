////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/AnimatedTexture.hpp>

#include <swift2d/application/Application.hpp>
#include <oglplus/images/png.hpp>
#include <oglplus/images/newton.hpp>
#include <istream>
#include <streambuf>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

AnimatedTexture::AnimatedTexture()
  : Texture() {

  TilesX.on_change().connect([&](unsigned){
    needs_update_ = true;
  });
  TilesY.on_change().connect([&](unsigned){
    needs_update_ = true;
  });
}

////////////////////////////////////////////////////////////////////////////////

AnimatedTexture::AnimatedTexture(std::string const& file_name, unsigned tiles_x, unsigned tiles_y)
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

void AnimatedTexture::bind(RenderContext const& context, unsigned location) const {
  if (needs_update_) {
    upload_to(context);
  }

  texture_->Active(location);
  context.gl.Bind(ose::_3D(), *texture_);
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedTexture::accept(SavableObjectVisitor& visitor) {
  Texture::accept(visitor);
  visitor.add_member("TilesX", TilesX);
  visitor.add_member("TilesY", TilesY);
}

////////////////////////////////////////////////////////////////////////////////

void AnimatedTexture::upload_to(RenderContext const& context) const {

  needs_update_ = false;

  if (texture_) {
    delete texture_;
  }

  int width(0), height(0), channels(0);
  unsigned char* map_data(load_texture_data(FileName(), &width, &height, &channels));

  if (map_data) {

    int tile_width  (width  / TilesX());
    int tile_height (height / TilesY());
    int tile_count  (TilesX() * TilesY());

    auto internal_format(channels > 3 ? ogl::InternalFormat::RGBA : ogl::InternalFormat::RGB);
    auto format(channels > 3 ? ogl::Format::RGBA : ogl::Format::RGB);

    texture_ = new ogl::Texture();

    auto tex = context.gl.Bound(ose::_3D(), *texture_);
    tex.Image3D(0, internal_format, tile_width, tile_height, tile_count, 0,
                format, ogl::DataType::UnsignedByte, nullptr);


    for (int tile_y(0); tile_y<TilesY(); ++tile_y) {
      for (int tile_x(0); tile_x<TilesX(); ++tile_x) {

        int tile_number(tile_y*TilesX() + tile_x);

        std::vector<unsigned char> data(tile_width*tile_height*channels);

        int d(0);
        int start(tile_y * tile_height * width + tile_width * tile_x);
        int stride(width);

        while (d<data.size()) {
          int x((d/channels)%(tile_width));
          int y((d/channels)/(tile_width));

          int pos(start + x + stride*y);

          for (int c(0); c<channels; ++c) {
            data[d++] = map_data[pos*channels + c];
          }
        }

        tex.SubImage3D(
          0, 0, 0, tile_number, tile_width, tile_height, 1,
          format, ogl::DataType::UnsignedByte, &data.front()
        );
      }
    }

    tex.GenerateMipmap()
       .MinFilter(ose::LinearMipmapLinear())
       .MagFilter(ose::Linear())
       .WrapS(ose::Repeat())
       .WrapT(ose::Repeat())
       .WrapR(ose::Repeat());

  } else {
    Logger::LOG_ERROR << "Failed to load texture \"" << FileName() << "\"!" << std::endl;
  }

  free_texture_data(map_data);
}

////////////////////////////////////////////////////////////////////////////////

}

