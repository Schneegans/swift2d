////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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
  , FrontBottomLeft (Color(0.f, 0.f, 0.f))
  , FrontTopLeft    (Color(0.f, 1.f, 0.f))
  , FrontTopRight   (Color(1.f, 1.f, 0.f))
  , FrontBottomRight(Color(1.f, 0.f, 0.f))
  , BackBottomLeft  (Color(0.f, 0.f, 1.f))
  , BackTopLeft     (Color(0.f, 1.f, 1.f))
  , BackTopRight    (Color(1.f, 1.f, 1.f))
  , BackBottomRight (Color(1.f, 0.f, 1.f))
  , update_data_(true) {

    connect_on_members();
  }

////////////////////////////////////////////////////////////////////////////////

ColorMap::ColorMap(std::string const& file_name, unsigned tiles_x, unsigned tiles_y)
  : Texture3D(file_name, tiles_x, tiles_y)
  , FrontBottomLeft (Color(0.f, 0.f, 0.f))
  , FrontTopLeft    (Color(0.f, 1.f, 0.f))
  , FrontTopRight   (Color(1.f, 1.f, 0.f))
  , FrontBottomRight(Color(1.f, 0.f, 0.f))
  , BackBottomLeft  (Color(0.f, 0.f, 1.f))
  , BackTopLeft     (Color(0.f, 1.f, 1.f))
  , BackTopRight    (Color(1.f, 1.f, 1.f))
  , BackBottomRight (Color(1.f, 0.f, 1.f))
  , update_data_(false) {

    connect_on_members();
  }

////////////////////////////////////////////////////////////////////////////////

ColorMap::ColorMap(
        Color const& fbl, Color const& ftl, Color const& ftr, Color const& fbr,
        Color const& bbl, Color const& btl, Color const& btr, Color const& bbr,
        int width, int height, int depth)
  : Texture3D()
  , FrontBottomLeft(fbl)
  , FrontTopLeft(ftl)
  , FrontTopRight(ftr)
  , FrontBottomRight(fbr)
  , BackBottomLeft(bbl)
  , BackTopLeft(btl)
  , BackTopRight(btr)
  , BackBottomRight(bbr)
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
  visitor.add_member("FrontBottomLeft", FrontBottomLeft);
  visitor.add_member("FrontTopLeft", FrontTopLeft);
  visitor.add_member("FrontTopRight", FrontTopRight);
  visitor.add_member("FrontBottomRight", FrontBottomRight);
  visitor.add_member("BackBottomLeft", BackBottomLeft);
  visitor.add_member("BackTopLeft", BackTopLeft);
  visitor.add_member("BackTopRight", BackTopRight);
  visitor.add_member("BackBottomRight", BackBottomRight);
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
    FrontBottomLeft(), FrontTopLeft(), FrontTopRight(), FrontBottomRight(),
    BackBottomLeft(), BackTopLeft(), BackTopRight(), BackBottomRight(),
    Width(), Height(), Depth()
  ));
}

////////////////////////////////////////////////////////////////////////////////

void ColorMap::connect_on_members() const {
  FrontBottomLeft.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  FrontTopLeft.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  FrontTopRight.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  FrontBottomRight.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  BackBottomLeft.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  BackTopLeft.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  BackTopRight.on_change().connect([&](Color const&){
    update_data_ = true;
    return true;
  });

  BackBottomRight.on_change().connect([&](Color const&){
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

