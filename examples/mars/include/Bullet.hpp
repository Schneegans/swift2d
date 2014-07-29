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
  Bullet(math::mat3 const& transform, SceneObjectPtr& scene) {

    math::vec3 position(1.5, 0, 1);
    position = transform * position;
    float rot = math::get_rotation(transform);

    Transform = math::make_translation(position.x(), position.y()) * math::make_scale(0.8) * math::make_rotation(rot);

    auto shape = PolygonCollisionShape::create();
         shape->Coordinates = {math::vec2(-1.f, -1.f), math::vec2(1.f, -1.f), math::vec2(0.0f, 1.f)};
    auto body = add<DynamicBodyComponent>();
         body->Density = 1;
         body->Shape = shape;

    auto tex = add<SpriteComponent>();
         tex->Depth = 0.0f;
         tex->Material = MaterialDatabase::instance()->get("bullet");
         tex->Transform = math::make_scale(1.0f);

    auto emitter = add<ParticleEmitterComponent>();
         emitter->Density = 50;
         emitter->PositionVariance = 1;
         emitter->Life = 1;
         emitter->LifeVariance = 0.7;
    scene->get_component<ParticleSystemComponent>()->add_emitter(emitter.get());

    auto deleter = add<TimedDeleteBehavior>();
         deleter->Life = 10.f;

    deleter->on_delete.connect([=](){
      scene->get_component<ParticleSystemComponent>()->remove_emitter(emitter.get());
    });
  }

  void shoot(SceneObjectPtr const& owner) {
    update(0);
    get_component<DynamicBodyComponent>()->apply_local_linear_impulse(math::vec2(0.01f, 0.0f));
    owner->get_component<DynamicBodyComponent>()->apply_local_linear_impulse(math::vec2(-0.002f, 0.0f));
  }

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "Bullet"; }
};

// -----------------------------------------------------------------------------

#endif // MARS_BULLET_HPP
