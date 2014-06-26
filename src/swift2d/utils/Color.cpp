////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/utils/Color.hpp>

// swift2d headers
#include <swift2d/math/random.hpp>

// external headers
#include <cmath>
#include <istream>
#include <ostream>
#include <algorithm>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Color::Color()
  : r_(0.0f)
  , g_(0.0f)
  , b_(0.0f) {}

////////////////////////////////////////////////////////////////////////////////

Color::Color(float r, float g, float b)
  : r_(r)
  , g_(g)
  , b_(b) {}

////////////////////////////////////////////////////////////////////////////////

float Color::h() const {

  if (s() > 0.0f) {
    float maxi = std::max(std::max(r_, g_), b_);
    float mini = std::min(std::min(r_, g_), b_);

    if (maxi == r_)
      return fmod(60.f * ((g_ - b_) / (maxi - mini)) + 360.f, 360.f);
    else if (maxi == g_)
      return fmod(60.f * (2 + (b_ - r_) / (maxi - mini)) + 360.f, 360.f);
    else
      return fmod(60.f * (4 + (r_ - g_) / (maxi - mini)) + 360.f, 360.f);
  } else
    return 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

float Color::s() const {
  if (v() == 0)
    return 0;
  else
    return ((v() - std::min(std::min(r_, g_), b_)) / v());
}

////////////////////////////////////////////////////////////////////////////////

void Color::r(float red) {
  r_ = math::clamp(red, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////

void Color::g(float green) {
  g_ = math::clamp(green, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////

void Color::b(float blue) {
  b_ = math::clamp(blue, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////

void Color::h(float hue) {
  set_hsv(hue, s(), v());
}

////////////////////////////////////////////////////////////////////////////////

void Color::s(float saturation) {
  set_hsv(h(), math::clamp(saturation, 0.0f, 1.0f), v());
}

////////////////////////////////////////////////////////////////////////////////

void Color::v(float value) {
  set_hsv(h(), s(), math::clamp(value, 0.0f, 1.0f));
}

////////////////////////////////////////////////////////////////////////////////

void Color::set_hsv(float hue, float saturation, float value) {

  if (saturation == 0) {
    r_ = value;
    g_ = value;
    b_ = value;
    return;
  }
  hue = fmod(hue, 360);
  hue /= 60;
  int i = int(floor(hue));
  float f = hue - i;

  switch (i) {
    case 0:
      r_ = value;
      g_ = value * (1 - saturation * (1 - f));
      b_ = value * (1 - saturation);
      break;
    case 1:
      r_ = value * (1 - saturation * f);
      g_ = value;
      b_ = value * (1 - saturation);
      break;
    case 2:
      r_ = value * (1 - saturation);
      g_ = value;
      b_ = value * (1 - saturation * (1 - f));
      break;
    case 3:
      r_ = value * (1 - saturation);
      g_ = value * (1 - saturation * f);
      b_ = value;
      break;
    case 4:
      r_ = value * (1 - saturation * (1 - f));
      g_ = value * (1 - saturation);
      b_ = value;
      break;
    default:
      r_ = value;
      g_ = value * (1 - saturation);
      b_ = value * (1 - saturation * f);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////

Color Color::inverted() const {

  Color inverted(*this);
  inverted.h(inverted.h() + 180.f);
  if (v() < 0.5f)
    inverted.v(1.f - inverted.v());
  return inverted;
}

////////////////////////////////////////////////////////////////////////////////

Color Color::brightened() const {

  Color brightened(*this);
  if (brightened.v() < 0.5f)
    brightened.v(0.5f);
  if (brightened.s() < 0.5f)
    brightened.s(0.5f);
  return brightened;
}

////////////////////////////////////////////////////////////////////////////////

math::vec3 Color::vec3() const {
  return math::vec3(r_, g_, b_);
}

////////////////////////////////////////////////////////////////////////////////

Color Color::random() {

  Color result(math::random::get(0.0f, 1.0f),
                 math::random::get(0.0f, 1.0f),
                 math::random::get(0.0f, 1.0f));
  result.s(result.s() + 0.5);
  result.v(result.v() + 0.5);
  return result;
}

////////////////////////////////////////////////////////////////////////////////

bool operator==(Color const& lhs, Color const& rhs) {
  return lhs.r() == rhs.r() && lhs.g() == rhs.g() && lhs.b() == rhs.b();
}

////////////////////////////////////////////////////////////////////////////////

bool operator!=(Color const& lhs, Color const& rhs) {
  return !(lhs == rhs);
}

////////////////////////////////////////////////////////////////////////////////

Color operator*(float const& lhs, Color rhs) {
  return Color(rhs.r() * lhs, rhs.g() * lhs, rhs.b() * lhs);
}

////////////////////////////////////////////////////////////////////////////////

Color operator*(Color const& lhs, float rhs) {
  return rhs * lhs;
}

////////////////////////////////////////////////////////////////////////////////

Color operator/(Color const& lhs, float rhs) {
  return Color(lhs.r() / rhs, lhs.g() / rhs, lhs.b() / rhs);
}

////////////////////////////////////////////////////////////////////////////////

Color operator+(Color const& lhs, Color const& rhs) {
  Color result;
  result.r(lhs.r() + rhs.r());
  result.g(lhs.g() + rhs.g());
  result.b(lhs.b() + rhs.b());
  return result;
}

////////////////////////////////////////////////////////////////////////////////

Color operator-(Color const& lhs, Color const& rhs) {
  Color result;
  result.r(lhs.r() - rhs.r());
  result.g(lhs.g() - rhs.g());
  result.b(lhs.b() - rhs.b());
  return result;
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, Color const& color) {
  os << color.r() << " " << color.g() << " " << color.b() << std::endl;
  return os;
}

////////////////////////////////////////////////////////////////////////////////

std::istream& operator>>(std::istream& is, Color& color) {
  float cur_val;
  is >> cur_val;
  color.r(cur_val);
  is >> cur_val;
  color.g(cur_val);
  is >> cur_val;
  color.b(cur_val);
  return is;
}

////////////////////////////////////////////////////////////////////////////////

}
