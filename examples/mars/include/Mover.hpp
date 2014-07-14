////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_MOVER_HPP
#define MARS_MOVER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

using namespace swift;

class Mover: public Component {
 public:
  Mover();

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Mover"; }

  virtual void accept(SavableObjectVisitor& visitor) {
    Component::accept(visitor);
  }

  virtual void update(double time);

  void set_camera(CameraComponentPtr const& camera) {
    camera_ = camera;
  }

 private:
  CameraComponentPtr camera_;
  float              camera_zoom_;
  math::vec2         camera_offset_;

};

// -----------------------------------------------------------------------------

#endif // MARS_MOVER_HPP