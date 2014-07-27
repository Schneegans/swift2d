////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_PLANET_HPP
#define MARS_PLANET_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

using namespace swift;

class Planet: public SceneObject {
 public:

  String        DiffuseTex;
  String        NormalTex;
  String        EmitTex;
  Float         Radius;
  Float         Density;
  Float         Shinyness;
  Float         Emit;
  ColorProperty Color;

  Planet();

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Planet"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);

 private:
  bool initialized_;
};

// -----------------------------------------------------------------------------

#endif // MARS_PLANET_HPP
