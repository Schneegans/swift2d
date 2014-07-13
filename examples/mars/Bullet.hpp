////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef MARS_BULLET_HPP
#define MARS_BULLET_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/swift2d.hpp>

using namespace swift;

class Bullet: public SceneObject {
 public:
  Bullet() {}
  Bullet(math::vec2 const& position) {

    Transform = math::make_translation(position) * math::make_scale(0.05);

    auto body = add<DynamicBodyComponent>();

    auto tex = add<SpriteComponent>();
         tex->Depth = 0.0f;
         tex->Material = MaterialDatabase::instance()->get("bullet");
         tex->Transform = math::make_scale(1.0f);

    auto deleter = add<TimedDeleteBehavior>();
         deleter->Life = 20.f;
  }

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Bullet"; }
};

// -----------------------------------------------------------------------------

#endif // MARS_BULLET_HPP
