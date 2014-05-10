////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef COLOR_3F_HPP
#define COLOR_3F_HPP

#include <swift2d/math.hpp>
#include <swift2d/events/Properties.hpp>

namespace swift {

/**
 * A struct for color handling.
 *
 * This class stores color values in RGB manner, but provides an HSV
 * interface as well.
 */

struct Color {
 public:

  /**
   * Constructor.
   *
   * This constructs a Color with all values set to 0 (black).
   */
  Color();

  /**
   * Constructor.
   *
   * This constructs a Color from given RGB values.
   *
   * \param red       The red value.
   * \param green     The green value.
   * \param blue      The blue value.
   */
  Color(float red, float green, float blue);

  ///@{
  /**
   * Returns a single Color value.
   */
  inline float r() const { return r_; }
  inline float g() const { return g_; }
  inline float b() const { return b_; }
  float h() const;
  float s() const;
  inline float v() const { return std::max(std::max(r_, g_), b_); }
  ///@}

  ///@{
  /**
   * Sets a single Color value.
   *
   * \param value    The new value to be set.
   */
  void r(float red);
  void g(float green);
  void b(float blue);
  void h(float hue);
  void s(float saturation);
  void v(float value);
  ///@}

  /**
   * Returns an inverted copy of the Color.
   *
   * \param color      The inverted copy of the Color.
   */
  Color const inverted() const;

  /**
   * Returns an inverted copy of the Color.
   *
   * \param color      The inverted copy of the Color.
   */
  Color const brightened() const;

  math::vec3 const vec3() const;

  /**
   * Returns a randomly generated Color.
   *
   * \return color      A randomly generated color.
   */
  static const Color random();

 private:
  void set_hsv(float hue, float saturation, float value);

  float r_, g_, b_;
};

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

typedef StreamableProperty<Color> ColorProperty;

}

#endif  //COLOR_3F_HPP
