////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_FIRE_HPP
#define MARS_FIRE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

using namespace swift;

class Fire: public SceneObject {
 public:

  Fire(math::vec2 const& pos = math::vec2());

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Fire"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);

 private:
  bool initialized_;
  static bool assets_loaded_;
};

// -----------------------------------------------------------------------------

#endif // MARS_FIRE_HPP
