////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_COLOR_MAP_HPP
#define SWIFT2D_COLOR_MAP_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/textures/Texture3D.hpp>
#include <swift2d/utils/Color.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// shared pointer type definition ----------------------------------------------
class ColorMap;
typedef std::shared_ptr<ColorMap>       ColorMapPtr;
typedef std::shared_ptr<const ColorMap> ConstColorMapPtr;
typedef Property<ColorMapPtr>           ColorMapProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL ColorMap : public Texture3D {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  ColorProperty Black;
  ColorProperty Green;
  ColorProperty Yellow;
  ColorProperty Red;
  ColorProperty Blue;
  ColorProperty Turqoise;
  ColorProperty White;
  ColorProperty Purple;
  Int32         Width;
  Int32         Height;
  Int32         Depth;

  // ---------------------------------------------------- construction interface
  template <typename... Args>
  static ColorMapPtr create(Args&& ... a) {
    return std::make_shared<ColorMap>(a...);
  }

  ColorMap();
  ColorMap(std::string const& file_name, unsigned tiles_x, unsigned tiles_y);

  ColorMap(Color const& fbl, Color const& ftl, Color const& ftr, Color const& fbr,
           Color const& bbl, Color const& btl, Color const& btr, Color const& bbr,
           int width, int height, int depth);

  static void save_to_file(std::string const& file_name,
                           Color const& fbl = Color(0.f, 0.f, 0.f),
                           Color const& ftl = Color(0.f, 1.f, 0.f),
                           Color const& ftr = Color(1.f, 1.f, 0.f),
                           Color const& fbr = Color(1.f, 0.f, 0.f),
                           Color const& bbl = Color(0.f, 0.f, 1.f),
                           Color const& btl = Color(0.f, 1.f, 1.f),
                           Color const& btr = Color(1.f, 1.f, 1.f),
                           Color const& bbr = Color(1.f, 0.f, 1.f),
                           int width = 16, int height = 16, int depth = 16);

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "ColorMap"; }

  // Binds the texture on the given context to the given location.
  virtual void bind(RenderContext const& context, unsigned location) const;

  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  mutable bool update_data_;
  void upload_to(RenderContext const& context) const;

  void update_data() const;
  void connect_on_members() const;

  static unsigned char* create_data(
          Color const& fbl, Color const& ftl, Color const& ftr, Color const& fbr,
          Color const& bbl, Color const& btl, Color const& btr, Color const& bbr,
          int width, int height, int depth);
};

}

#endif // SWIFT2D_COLOR_MAP_HPP
