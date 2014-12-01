////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/Physics.hpp>

#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/math/transformations.hpp>

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

class SwiftContactListener : public b2ContactListener {
 public:
  void BeginContact(b2Contact* contact) {
    auto body_a(contact->GetFixtureA()->GetBody());
    auto body_b(contact->GetFixtureB()->GetBody());

    b2WorldManifold p;
    contact->GetWorldManifold(&p);
    math::vec2 point(p.points[0].x, p.points[0].y);

    if (body_b->GetType() == b2_staticBody && body_a->GetType() == b2_dynamicBody) {
      auto a = static_cast<DynamicBodyComponent*>(body_a->GetUserData());
      auto b = static_cast<StaticBodyComponent*>(body_b->GetUserData());


      a->start_contact_with_static.emit(b, point);
      b->start_contact_with_dynamic.emit(a, point);

    } else if (body_b->GetType() == b2_dynamicBody && body_a->GetType() == b2_staticBody) {
      auto a = static_cast<DynamicBodyComponent*>(body_b->GetUserData());
      auto b = static_cast<StaticBodyComponent*>(body_a->GetUserData());

      a->start_contact_with_static.emit(b, point);
      b->start_contact_with_dynamic.emit(a, point);

    } else if (body_b->GetType() == b2_dynamicBody && body_a->GetType() == b2_dynamicBody) {
      auto a = static_cast<DynamicBodyComponent*>(body_a->GetUserData());
      auto b = static_cast<DynamicBodyComponent*>(body_b->GetUserData());

      a->start_contact_with_dynamic.emit(b, point);
      b->start_contact_with_dynamic.emit(a, point);

    }

  }

  void EndContact(b2Contact* contact) {
    auto body_a(contact->GetFixtureA()->GetBody());
    auto body_b(contact->GetFixtureB()->GetBody());

    b2WorldManifold p;
    contact->GetWorldManifold(&p);
    math::vec2 point(p.points[0].x, p.points[0].y);

    if (body_b->GetType() == b2_staticBody && body_a->GetType() == b2_dynamicBody) {
      auto a = static_cast<DynamicBodyComponent*>(body_a->GetUserData());
      auto b = static_cast<StaticBodyComponent*>(body_b->GetUserData());

      a->end_contact_with_static.emit(b, point);
      b->end_contact_with_dynamic.emit(a, point);
    }

    else if (body_b->GetType() == b2_dynamicBody && body_a->GetType() == b2_dynamicBody) {
      auto a = static_cast<DynamicBodyComponent*>(body_a->GetUserData());
      auto b = static_cast<DynamicBodyComponent*>(body_b->GetUserData());

      a->end_contact_with_dynamic.emit(b, point);
      b->end_contact_with_dynamic.emit(a, point);
    }

  }
  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}
  void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Physics::Physics()
  : world_(new b2World(b2Vec2(0.f, 0.f)))
  , contact_listener_(new SwiftContactListener())
  , gravity_map_(nullptr) {

  world_->SetContactListener(contact_listener_);
  world_->SetAllowSleeping(false);
}

////////////////////////////////////////////////////////////////////////////////

Physics::~Physics() {
  delete world_;
  delete contact_listener_;
}

////////////////////////////////////////////////////////////////////////////////

void Physics::update(double time) {

  auto body(world_->GetBodyList());

  while (body) {
    auto body_pos = body->GetWorldCenter();

    if (body->IsAwake() && body->GetType() == b2_dynamicBody) {
      for (auto source: gravity_sources_) {
        auto transform(source->get_user()->WorldTransform());
        auto pos(math::get_translation(transform));
        auto scale(math::get_scale(transform));
        float mass(source->Mass());
        math::vec2 direction(pos - math::vec2(body_pos.x, body_pos.y));
        float distance(math::get_length_squared(direction));
        if (distance > 1.f) {
          auto b(static_cast<DynamicBodyComponent*>(body->GetUserData()));
          direction = direction * mass * b->GravityScale() / distance;
          b->apply_global_force(direction);
        }
      }

      for (auto const& shock: shock_waves_) {
        b2Vec2 dist(body_pos - b2Vec2(shock.x(), shock.y()));
        float length(dist.LengthSquared());
        if (length > 0) {
          dist *= shock.z()/length;
          body->ApplyLinearImpulse(dist, body_pos, true);
        }
      }
    }

    body = body->GetNext();
  }

  shock_waves_.clear();

  world_->Step(time, 6, 2);
}

////////////////////////////////////////////////////////////////////////////////

b2Body* Physics::add(DynamicBodyComponent* body) {
  auto transform(body->get_user()->WorldTransform());
  math::vec2 pos(math::get_translation(transform));
  math::vec2 scale(math::get_scale(transform));

  b2FixtureDef fixtureDef;

  b2Shape* shape;
  if (!body->Shape()) {
    LOG_WARNING << "Failed to add DynamicBodyComponent: "
                        << "No CollisionShape attached!" << std::endl;
    shape = new b2CircleShape();
  } else {
    shape = body->Shape()->get_shape(transform);
  }
  fixtureDef.density = body->Mass() / scale.x() / scale.y();
  fixtureDef.friction = body->Friction();
  fixtureDef.restitution = body->Restitution();
  fixtureDef.filter.groupIndex = body->Group();
  fixtureDef.filter.maskBits = body->Mask();
  fixtureDef.filter.categoryBits = body->Category();
  fixtureDef.shape = shape;

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos.x(), pos.y());
  bodyDef.angle = math::get_rotation(transform);

  b2Body* result = world_->CreateBody(&bodyDef);

  result->SetUserData(body);
  result->CreateFixture(&fixtureDef);
  result->SetLinearDamping(body->LinearDamping());
  result->SetAngularDamping(body->AngularDamping());

  delete shape;

  return result;
}

////////////////////////////////////////////////////////////////////////////////

b2Body* Physics::add(StaticBodyComponent* body) {
  auto transform(body->get_user()->WorldTransform());
  math::vec2 pos(math::get_translation(transform));
  float rot(math::get_rotation(transform));

  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(pos.x(), pos.y());
  bodyDef.angle = rot;
  b2Body* result = world_->CreateBody(&bodyDef);

  result->SetUserData(body);

  b2FixtureDef fixtureDef;
  fixtureDef.friction = body->Friction();
  fixtureDef.restitution = body->Restitution();
  fixtureDef.filter.groupIndex = body->Group();
  fixtureDef.filter.maskBits = body->Mask();
  fixtureDef.filter.categoryBits = body->Category();

  b2Shape* shape;

  if (!body->Shape()) {
    LOG_WARNING << "Failed to add DynamicBodyComponent: "
                        << "No CollisionShape attached!" << std::endl;
    shape = new b2CircleShape();
  } else {
    shape = body->Shape()->get_shape(transform);
  }
  fixtureDef.shape = shape;

  result->CreateFixture(&fixtureDef);

  delete shape;

  return result;
}

////////////////////////////////////////////////////////////////////////////////

void Physics::add(GravitySourceComponent* source) {
  gravity_sources_.insert(source);
}

////////////////////////////////////////////////////////////////////////////////

void Physics::add_shock_wave(math::vec2 const& location, float strength) {
  shock_waves_.push_back(math::vec3(location.x(), location.y(), strength));
}

////////////////////////////////////////////////////////////////////////////////

void Physics::remove(b2Body* body) {
  world_->DestroyBody(body);
}

////////////////////////////////////////////////////////////////////////////////

void Physics::remove(GravitySourceComponent* source) {
  gravity_sources_.erase(source);
}

////////////////////////////////////////////////////////////////////////////////

void Physics::create_gravity_map(RenderContext const& ctx) {
  if (gravity_map_) delete gravity_map_;
  gravity_map_ = new GravityMap(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void Physics::clear_gravity_map(RenderContext const& ctx) {
  if (gravity_map_) delete gravity_map_;
  gravity_map_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void Physics::bind_gravity_map(RenderContext const& ctx, int location) {
  gravity_map_->bind(ctx, location);
}

////////////////////////////////////////////////////////////////////////////////

void Physics::update_gravity_map(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  gravity_map_->process(scene, ctx);
}

////////////////////////////////////////////////////////////////////////////////

}
