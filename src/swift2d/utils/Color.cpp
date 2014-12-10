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
  : val_(0, 0, 0, 1) {}

////////////////////////////////////////////////////////////////////////////////

Color::Color(float r, float g, float b, float a)
  : val_(r, g, b, a) {}

////////////////////////////////////////////////////////////////////////////////

float Color::h() const {

  if (s() > 0.0f) {
    float maxi = std::max(std::max(val_[0], val_[1]), val_[2]);
    float mini = std::min(std::min(val_[0], val_[1]), val_[2]);

    if (maxi == val_[0])
      return fmod(60.f * ((val_[1] - val_[2]) / (maxi - mini)) + 360.f, 360.f);
    else if (maxi == val_[1])
      return fmod(60.f * (2 + (val_[2] - val_[0]) / (maxi - mini)) + 360.f, 360.f);
    else
      return fmod(60.f * (4 + (val_[0] - val_[1]) / (maxi - mini)) + 360.f, 360.f);
  } else
    return 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

float Color::s() const {
  if (v() == 0)
    return 0;
  else
    return ((v() - std::min(std::min(val_[0], val_[1]), val_[2])) / v());
}

////////////////////////////////////////////////////////////////////////////////

void Color::r(float red) {
  val_[0] = math::clamp(red, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////

void Color::g(float green) {
  val_[1] = math::clamp(green, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////

void Color::b(float blue) {
  val_[2] = math::clamp(blue, 0.0f, 1.0f);
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

void Color::a(float alpha) {
  val_[3] = math::clamp(alpha, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////

void Color::set_hsv(float hue, float saturation, float value) {

  if (saturation == 0) {
    val_[0] = value;
    val_[1] = value;
    val_[2] = value;
    return;
  }
  hue = fmod(hue, 360);
  hue /= 60;
  int i = int(floor(hue));
  float f = hue - i;

  switch (i) {
    case 0:
      val_[0] = value;
      val_[1] = value * (1 - saturation * (1 - f));
      val_[2] = value * (1 - saturation);
      break;
    case 1:
      val_[0] = value * (1 - saturation * f);
      val_[1] = value;
      val_[2] = value * (1 - saturation);
      break;
    case 2:
      val_[0] = value * (1 - saturation);
      val_[1] = value;
      val_[2] = value * (1 - saturation * (1 - f));
      break;
    case 3:
      val_[0] = value * (1 - saturation);
      val_[1] = value * (1 - saturation * f);
      val_[2] = value;
      break;
    case 4:
      val_[0] = value * (1 - saturation * (1 - f));
      val_[1] = value * (1 - saturation);
      val_[2] = value;
      break;
    default:
      val_[0] = value;
      val_[1] = value * (1 - saturation);
      val_[2] = value * (1 - saturation * f);
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
  return val_.xyz();
}

////////////////////////////////////////////////////////////////////////////////

math::vec4 const& Color::vec4() const {
  return val_;
}

////////////////////////////////////////////////////////////////////////////////

std::string Color::html_rgb() const {
  std::stringstream stream;
  stream << "rgb("
         << static_cast<int>(r() * 255) << ", "
         << static_cast<int>(g() * 255) << ", "
         << static_cast<int>(b() * 255) << ")";

  return stream.str();
}

////////////////////////////////////////////////////////////////////////////////

void Color::html_rgb(std::string const& val) {
  std::string str(val);
  std::string cropped = str.substr(str.find_first_of("(")+1, str.find_first_of(")")-1);

  std::stringstream stream(cropped);
  std::string comma;
  float r;
  float g;
  float b;
  stream >> r >> comma >> g >> comma >> b;

  this->r(r/255.f);
  this->g(g/255.f);
  this->b(b/255.f);
}

////////////////////////////////////////////////////////////////////////////////

Color Color::random() {

  Color result(math::random::get(0.0f, 1.0f),
               math::random::get(0.0f, 1.0f),
               math::random::get(0.0f, 1.0f), 1.f);
  result.s(result.s() + 0.5);
  result.v(result.v() + 0.5);
  return result;
}

////////////////////////////////////////////////////////////////////////////////

float Color::operator[](unsigned rhs) const {
  return val_[rhs];
}

////////////////////////////////////////////////////////////////////////////////

float& Color::operator[](unsigned rhs) {
  return val_[rhs];
}

////////////////////////////////////////////////////////////////////////////////

bool operator==(Color const& lhs, Color const& rhs) {
  return lhs.r() == rhs.r() && lhs.g() == rhs.g() && lhs.b() == rhs.b() && lhs.a() == rhs.a();
}

////////////////////////////////////////////////////////////////////////////////

bool operator!=(Color const& lhs, Color const& rhs) {
  return !(lhs == rhs);
}

////////////////////////////////////////////////////////////////////////////////

Color operator*(float const& lhs, Color rhs) {
  return Color(rhs.r() * lhs, rhs.g() * lhs, rhs.b() * lhs, rhs.a());
}

////////////////////////////////////////////////////////////////////////////////

Color operator*(Color const& lhs, float rhs) {
  return rhs * lhs;
}

////////////////////////////////////////////////////////////////////////////////

Color operator/(Color const& lhs, float rhs) {
  return Color(lhs.r() / rhs, lhs.g() / rhs, lhs.b() / rhs, lhs.a());
}

////////////////////////////////////////////////////////////////////////////////

Color operator+(Color const& lhs, Color const& rhs) {
  Color result;
  result.r(lhs.r() + rhs.r());
  result.g(lhs.g() + rhs.g());
  result.b(lhs.b() + rhs.b());
  result.a(0.5*(lhs.a() + rhs.a()));
  return result;
}

////////////////////////////////////////////////////////////////////////////////

Color operator-(Color const& lhs, Color const& rhs) {
  Color result;
  result.r(lhs.r() - rhs.r());
  result.g(lhs.g() - rhs.g());
  result.b(lhs.b() - rhs.b());
  result.a(0.5*(lhs.a() + rhs.a()));
  return result;
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, Color const& color) {
  os << color.vec4();
  return os;
}

////////////////////////////////////////////////////////////////////////////////

std::istream& operator>>(std::istream& is, Color& color) {
  math::vec4 val;
  is >> val;

  color.r(val[0]);
  color.g(val[1]);
  color.b(val[2]);
  color.a(val[3]);

  return is;
}

////////////////////////////////////////////////////////////////////////////////

}
