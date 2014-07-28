////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_ASTEROID_HPP
#define MARS_ASTEROID_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

using namespace swift;

class Asteroid: public SceneObject {
 public:

  String        DiffuseTex;
  String        NormalTex;
  String        EmitTex;
  Float         Radius;
  Float         Density;
  Float         Shinyness;
  Float         Emit;
  ColorProperty Color;

  Asteroid();

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Asteroid"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);

 private:
  bool initialized_;
};

// -----------------------------------------------------------------------------

#endif // MARS_ASTEROID_HPP
