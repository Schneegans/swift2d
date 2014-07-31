////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_EXPLOSION_HPP
#define MARS_EXPLOSION_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

using namespace swift;

class Explosion: public SceneObject {
 public:

  Explosion(math::vec2 const& pos = math::vec2());

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Explosion"; }

  virtual void update(double time);

  virtual void accept(SavableObjectVisitor& visitor);

 private:
  bool initialized_;
  static bool assets_loaded_;
};

// -----------------------------------------------------------------------------

#endif // MARS_EXPLOSION_HPP
