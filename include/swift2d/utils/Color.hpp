////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
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

struct Color {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface
  // This constructs a Color with all values set to 0 (black).
  Color();

  // This constructs a Color from given RGB values.
  Color(float red, float green, float blue, float alpha = 1.f);

  // Returns a randomly generated Color.
  static Color random();

  // ------------------------------------------------------------ public methods
  // Returns a single Color value.
  inline float r() const { return r_; }
  inline float g() const { return g_; }
  inline float b() const { return b_; }
  float h() const;
  float s() const;
  inline float v() const { return std::max(std::max(r_, g_), b_); }
  inline float a() const { return a_; }

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

  math::vec3 vec3() const;
  math::vec4 vec4() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void set_hsv(float hue, float saturation, float value);

  float r_, g_, b_, a_;
};

// ------------------------------------------------------------------- operators
bool operator==(Color const& lhs, Color const& rhs);
bool operator!=(Color const& lhs, Color const& rhs);

// Multiplication of a color with a float.
Color operator*(float const& lhs, Color rhs);
Color operator*(Color const& lhs, float rhs);

// Addition of two colors. Clamped.
Color operator+(Color const& lhs, Color const& rhs);

// Subtraction of two colors. Clamped.
Color operator-(Color const& lhs, Color const& rhs);

// Division of a color by a float.
Color operator/(Color const& lhs, float rhs);

std::ostream& operator<<(std::ostream& os, Color const& color);
std::istream& operator>>(std::istream& is, Color& color);

typedef Property<Color> ColorProperty;

}

#endif // SWIFT2D_COLOR_HPP
