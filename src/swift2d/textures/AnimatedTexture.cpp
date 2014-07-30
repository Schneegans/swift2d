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

  int map_width(0), map_height(0), channels(0);
  unsigned char* map_data(load_texture_data(FileName(), &map_width, &map_height, &channels));

  if (map_data) {

    int width  (map_width  / TilesX());
    int height (map_height / TilesY());
    int depth  (TilesX() * TilesY());

    auto internal_format(channels > 3 ? ogl::InternalFormat::RGBA : ogl::InternalFormat::RGB);
    auto format(channels > 3 ? ogl::Format::RGBA : ogl::Format::RGB);

    texture_ = new ogl::Texture();

    auto tex = context.gl.Bound(ose::_3D(), *texture_);
    tex.Image3D(0, internal_format, width, height, depth, 0, format,
                ogl::DataType::UnsignedByte, nullptr);


    for (int map_y(0); map_y<TilesY(); ++map_y) {
      for (int map_x(0); map_x<TilesX(); ++map_x) {

        int layer(map_y*TilesX() + map_x);

        std::vector<unsigned char> data(width*height*channels);

        int d(0);
        int start(map_y * height * channels + width * map_x * channels);
        int stride(map_width * channels);

        while (d<data.size()) {
          int x(d%width);
          int y(d/width);

          int pos(start + x + stride*y);

          for (int c(0); c<channels; ++c) {
            data[d++] = map_data[pos + c];
          }
        }

        tex.SubImage3D(
          0, 0, 0, layer, width, height, 1,
          format, ogl::DataType::UnsignedByte, &data.front()
        );
      }
    }

    tex.GenerateMipmap()
       .MinFilter(ose::LinearMipmapLinear())
       .MagFilter(ose::Linear())
       .WrapS(ose::Repeat())
       .WrapT(ose::Repeat())
       .WrapR(ose::ClampToEdge());
  } else {
    Logger::LOG_ERROR << "Failed to load texture \"" << FileName() << "\"!" << std::endl;
  }

  free_texture_data(map_data);
}

////////////////////////////////////////////////////////////////////////////////

}

