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
#include <istream>
#include <streambuf>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ColorMap::ColorMap()
  : Texture3D()
  , update_data_(true) {

    connect_on_members();
  }

////////////////////////////////////////////////////////////////////////////////

ColorMap::ColorMap(std::string const& file_name, unsigned tiles_x, unsigned tiles_y)
  : Texture3D(file_name, tiles_x, tiles_y)
  , update_data_(true) {

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
  TilesX = 1;
  TilesY = depth;

  set_width(width);
  set_height(height * depth);
  set_channels(4);

  connect_on_members();

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
  int channels(4);

  auto data = new unsigned char[Depth() * Width() * Height() * channels];

  for (int z(0); z < Depth(); ++z) {
    float c(z * 1.f/(Depth()-1));

    for (int y(0); y < Height(); ++y) {
      float b(y * 1.f/(Height()-1));

      for (int x(0); x < Width(); ++x ) {
        float a(x * 1.f/(Width()-1));
        math::vec4 color_fb((1.f - a) * FrontBottomLeft().vec4() +
                                   a  * FrontBottomRight().vec4());
        math::vec4 color_ft((1.f - a) * FrontTopLeft().vec4() +
                                   a  * FrontTopRight().vec4());
        math::vec4 color_bb((1.f - a) * BackBottomLeft().vec4() +
                                   a  * BackBottomRight().vec4());
        math::vec4 color_bt((1.f - a) * BackTopLeft().vec4() +
                                   a  * BackTopRight().vec4());

        math::vec4 color_f((1.f - b) * color_fb + b * color_ft);
        math::vec4 color_b((1.f - b) * color_bb + b * color_bt);

        math::vec4 color((1.f - c) * color_f + c * color_b);

        for (int chan(0); chan < channels; ++chan) {
          data[(x + y*Width() + z*Height()*Width())*channels + chan] = color[chan] * 255;
        }
      }
    }
  }

  set_data(data);

}

////////////////////////////////////////////////////////////////////////////////

void ColorMap::connect_on_members() const {
  FrontBottomLeft.on_change().connect([&](Color const&){
    update_data_ = true;
  });

  FrontTopLeft.on_change().connect([&](Color const&){
    update_data_ = true;
  });

  FrontTopRight.on_change().connect([&](Color const&){
    update_data_ = true;
  });

  FrontBottomRight.on_change().connect([&](Color const&){
    update_data_ = true;
  });

  BackBottomLeft.on_change().connect([&](Color const&){
    update_data_ = true;
  });

  BackTopLeft.on_change().connect([&](Color const&){
    update_data_ = true;
  });

  BackTopRight.on_change().connect([&](Color const&){
    update_data_ = true;
  });

  BackBottomRight.on_change().connect([&](Color const&){
    update_data_ = true;
  });

  Width.on_change().connect([&](int){
    update_data_ = true;
  });

  Height.on_change().connect([&](int){
    update_data_ = true;
  });

  Depth.on_change().connect([&](int){
    update_data_ = true;
  });
}

}

