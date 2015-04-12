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

#ifndef SWIFT2D_COLOR_HPP
#define SWIFT2D_COLOR_HPP

#include <swift2d/math.hpp>
#include <swift2d/properties.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
// This class stores color values in RGB manner, but provides an HSV          //
// interface as well.                                                         //
////////////////////////////////////////////////////////////////////////////////

class SWIFT_DLL Color {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  // This constructs a Color with all values set to 0 (black).
  Color();

  Color(std::string const& html_rgba);

  // This constructs a Color from given RGB values.
  Color(float red, float green, float blue, float alpha = 1.f);

  // Returns a randomly generated Color.
  static Color random();

  // ------------------------------------------------------------ public methods
  // Returns a single Color value.
  float r() const { return val_[0]; }
  float g() const { return val_[1]; }
  float b() const { return val_[2]; }
  float h() const;
  float s() const;
  float v() const { return std::max(std::max(val_[0], val_[1]), val_[2]); }
  float a() const { return val_[3]; }

  // Sets a single Color value.
  void r(float red);
  void g(float green);
  void b(float blue);
  void h(float hue);
  void s(float saturation);
  void v(float value);
  void a(float alpha);

  // Returns an inverted copy of the Color.
  Color inverted() const;

  // Returns an inverted copy of the Color.
  Color brightened() const;

  math::vec3        vec3() const;
  math::vec4 const& vec4() const;
  std::string       html_rgba() const;
  void              html_rgba(std::string const& val);

  float operator[](unsigned rhs) const;
  float& operator[](unsigned rhs);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void set_hsv(float hue, float saturation, float value);

  math::vec4 val_;
};

// ------------------------------------------------------------------- operators
SWIFT_DLL bool operator==(Color const& lhs, Color const& rhs);
SWIFT_DLL bool operator!=(Color const& lhs, Color const& rhs);

// Multiplication of a color with a float.
SWIFT_DLL Color operator*(float const& lhs, Color rhs);
SWIFT_DLL Color operator*(Color const& lhs, float rhs);

// Addition of two colors. Clamped.
SWIFT_DLL Color operator+(Color const& lhs, Color const& rhs);

// Subtraction of two colors. Clamped.
SWIFT_DLL Color operator-(Color const& lhs, Color const& rhs);

// Division of a color by a float.
SWIFT_DLL Color operator/(Color const& lhs, float rhs);

SWIFT_DLL std::ostream& operator<<(std::ostream& os, Color const& color);
SWIFT_DLL std::istream& operator>>(std::istream& is, Color& color);


typedef Property<Color> ColorProperty;

}

#endif // SWIFT2D_COLOR_HPP
