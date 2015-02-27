////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/textures/ColorMap.hpp>

#include <oglplus/images/png.hpp>
#include <oglplus/images/newton.hpp>
#include <stb_image/stb_image_write.h>

#include <istream>
#include <streambuf>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ColorMap::ColorMap()
  : Texture3D()
  , Black     (Color(0.f, 0.f, 0.f))
  , Green     (Color(0.f, 1.f, 0.f))
  , Yellow    (Color(1.f, 1.f, 0.f))
  , Red       (Color(1.f, 0.f, 0.f))
  , Blue      (Color(0.f, 0.f, 1.f))
  , Turqoise  (Color(0.f, 1.f, 1.f))
  , White     (Color(1.f, 1.f, 1.f))
  , Purple    (Color(1.f, 0.f, 1.f))
  , Width(16)
  , Height(16)
  , Depth(16)
  , update_data_(true) {

    tiles_x_ = 1;
    tiles_y_ = 16;

    set_width(16);
    set_height(16 * 16);
    set_channels(4);

    connect_on_members();
  }

////////////////////////////////////////////////////////////////////////////////

ColorMap::ColorMap(std::string const& file_name, unsigned tiles_x, unsigned tiles_y)
  : Texture3D(file_name, tiles_x, tiles_y)
  , Black (Color(0.f, 0.f, 0.f))
  , Green    (Color(0.f, 1.f, 0.f))
  , Yellow   (Color(1.f, 1.f, 0.f))
  , Red(Color(1.f, 0.f, 0.f))
  , Blue  (Color(0.f, 0.f, 1.f))
  , Turqoise     (Color(0.f, 1.f, 1.f))
  , White    (Color(1.f, 1.f, 1.f))
  , Purple (Color(1.f, 0.f, 1.f))
  , update_data_(false) {

    connect_on_members();
  }

////////////////////////////////////////////////////////////////////////////////

ColorMap::ColorMap(
        Color const& fbl, Color const& ftl, Color const& ftr, Color const& fbr,
        Color const& bbl, Color const& btl, Color const& btr, Color const& bbr,
        int width, int height, int depth)
  : Texture3D()
  , Black(fbl)
  , Green(ftl)
  , Yellow(ftr)
  , Red(fbr)
  , Blue(bbl)
  , Turqoise(btl)
  , White(btr)
  , Purple(bbr)
  , Width(width)
  , Height(height)
  , Depth(depth)
  , update_data_(true)
{
  tiles_x_ = 1;
  tiles_y_ = depth;

  set_width(width);
  set_height(height * depth);
  set_channels(4);

  connect_on_members();

}

////////////////////////////////////////////////////////////////////////////////

void ColorMap::save_to_file(
          std::string const& file_name,
          Color const& fbl, Color const& ftl, Color const& ftr, Color const& fbr,
          Color const& bbl, Color const& btl, Color const& btr, Color const& bbr,
          int width, int height, int depth) {

  auto data(create_data(fbl, ftl, ftr, fbr,
                        bbl, btl, btr, bbr,
                        width, height, depth));

  stbi_write_png(file_name.c_str(), width, height*depth, 4,
                 data, width * 4 * sizeof(unsigned char));
}
////////////////////////////////////////////////////////////////////////////////

void ColorMap::bind(RenderContext const& ctx, unsigned location) const {
  if (update_data_) {
    update_data_ = false;
    update_data();
    upload_to(ctx);
  }

  Texture3D::bind(ctx, location);
}

////////////////////////////////////////////////////////////////////////////////

void ColorMap::accept(SavableObjectVisitor& visitor) {
  Texture3D::accept(visitor);
  visitor.add_member("Black", Black);
  visitor.add_member("Green", Green);
  visitor.add_member("Yellow", Yellow);
  visitor.add_member("Red", Red);
  visitor.add_member("Blue", Blue);
  visitor.add_member("Turqoise", Turqoise);
  visitor.add_member("White", White);
  visitor.add_member("Purple", Purple);
  visitor.add_member("Width", Width);
  visitor.add_member("Height", Height);
  visitor.add_member("Depth", Depth);
}

////////////////////////////////////////////////////////////////////////////////

void ColorMap::upload_to(RenderContext const& ctx) const {
  Texture3D::upload_to(ctx, false);
}

////////////////////////////////////////////////////////////////////////////////

void ColorMap::update_data() const {
  set_data(create_data(
    Black(), Green(), Yellow(), Red(),
    Blue(), Turqoise(), White(), Purple(),
    Width(), Height(), Depth()
  ));
}

////////////////////////////////////////////////////////////////////////////////

void ColorMap::connect_on_members() const {
  Black.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  Green.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  Yellow.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  Red.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  Blue.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  Turqoise.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  White.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  Purple.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  Width.on_change().connect([&](int){
    update_data_ = true;
    return true;
  });

  Height.on_change().connect([&](int){
    update_data_ = true;
    return true;
  });

  Depth.on_change().connect([&](int){
    update_data_ = true;
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

unsigned char* ColorMap::create_data(
          Color const& fbl, Color const& ftl, Color const& ftr, Color const& fbr,
          Color const& bbl, Color const& btl, Color const& btr, Color const& bbr,
          int width, int height, int depth) {

  int channels(4);

  auto data = new unsigned char[depth * width * height * channels];

  for (int z(0); z < depth; ++z) {
    float c(z * 1.f/(depth-1));

    for (int y(0); y < height; ++y) {
      float b(y * 1.f/(height-1));

      for (int x(0); x < width; ++x ) {
        float a(x * 1.f/(width-1));
        math::vec4 color_fb((1.f - a) * fbl.vec4() +
                                   a  * fbr.vec4());
        math::vec4 color_ft((1.f - a) * ftl.vec4() +
                                   a  * ftr.vec4());
        math::vec4 color_bb((1.f - a) * bbl.vec4() +
                                   a  * bbr.vec4());
        math::vec4 color_bt((1.f - a) * btl.vec4() +
                                   a  * btr.vec4());

        math::vec4 color_f((1.f - b) * color_fb + b * color_ft);
        math::vec4 color_b((1.f - b) * color_bb + b * color_bt);

        math::vec4 color((1.f - c) * color_f + c * color_b);

        for (int chan(0); chan < channels; ++chan) {
          data[(x + y*width + z*height*width)*channels + chan] = color[chan] * 255;
        }
      }
    }
  }

  return data;
}

}

