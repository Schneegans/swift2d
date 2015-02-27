////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/Physics.hpp>

#include <swift2d/components/LifeComponent.hpp>
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

    auto a = static_cast<PhysicsBodyComponent*>(body_a->GetUserData());
    auto b = static_cast<PhysicsBodyComponent*>(body_b->GetUserData());

    a->start_contact.emit(a, b, point);
    b->start_contact.emit(b, a, point);
  }

  void EndContact(b2Contact* contact) {
    auto body_a(contact->GetFixtureA()->GetBody());
    auto body_b(contact->GetFixtureB()->GetBody());

    b2WorldManifold p;
    contact->GetWorldManifold(&p);
    math::vec2 point(p.points[0].x, p.points[0].y);

    auto a = static_cast<PhysicsBodyComponent*>(body_a->GetUserData());
    auto b = static_cast<PhysicsBodyComponent*>(body_b->GetUserData());

    a->end_contact.emit(a, b, point);
    b->end_contact.emit(b, a, point);
  }
  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}
  void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}
};

////////////////////////////////////////////////////////////////////////////////

class SwiftRayCastCallback : public b2RayCastCallback {

 public:
  std::vector<PhysicsBodyComponent*> hits;
  std::vector<math::vec2>            hit_points;
  std::vector<math::vec2>            hit_normals;

  virtual float ReportFixture (b2Fixture* fixture, b2Vec2 const& point,
                               b2Vec2 const& normal, float fraction) {

    hits.push_back(static_cast<PhysicsBodyComponent*>(fixture->GetBody()->GetUserData()));
    hit_points.push_back(math::vec2(point.x, point.y));
    hit_normals.push_back(math::vec2(normal.x, normal.y));

    return 1.f;
  }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Physics::Physics()
  : Gravity(math::vec2(0.f, 0.f))
  , world_(new b2World(b2Vec2(0.f, 0.f)))
  , contact_listener_(new SwiftContactListener())
  , gravity_map_(nullptr) {

  world_->SetContactListener(contact_listener_);
  world_->SetAllowSleeping(false);

  Gravity.on_change().connect([this](math::vec2 const& val) {
    world_->SetGravity(b2Vec2(val.x(), val.y()));
    return true;
  });
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
        float distance(math::get_length(direction));
        if (distance > 1.f) {
          auto b(static_cast<PhysicsBodyComponent*>(body->GetUserData()));
          direction = direction * mass * b->GravityScale() / (distance*distance*distance);
          b->apply_global_force(direction, false);
        }
      }

      for (auto const& shock: shock_waves_) {
        b2Vec2 dist(body_pos - b2Vec2(shock.location.x(), shock.location.y()));
        float length(dist.LengthSquared());
        if (length > 0 && length < shock.radius*shock.radius) {
          dist *= shock.strength/(length+1.f)*2;
          body->ApplyLinearImpulse(dist, body_pos, true);

          auto b(static_cast<PhysicsBodyComponent*>(body->GetUserData()));
          auto life = b->get_user()->get_component<LifeComponent>();
          if (life) {
            float damage((1.f - std::sqrt(length)/shock.radius) * shock.damage);
            life->decrease(damage, shock.damage_source, math::vec2(dist.x, dist.y)*shock.strength*0.1f);
          }
        }
      }
    }

    body = body->GetNext();
  }

  shock_waves_.clear();

  world_->Step(time, 6, 2);
}

////////////////////////////////////////////////////////////////////////////////

b2Body* Physics::add(PhysicsBodyComponent* body) {
  auto transform(body->get_user()->WorldTransform());
  math::vec2 pos(math::get_translation(transform));
  math::vec2 scale(math::get_scale(transform));
  float      rot(math::get_rotation(transform));

  b2BodyDef bodyDef;
  bodyDef.type = static_cast<b2BodyType>(body->BodyType());
  bodyDef.position.Set(pos.x(), pos.y());
  bodyDef.angle = rot;
  bodyDef.awake = !body->Sleep();
  b2Body* result = world_->CreateBody(&bodyDef);

  b2FixtureDef fixtureDef;
  fixtureDef.density = body->Mass() / scale.x() / scale.y();
  fixtureDef.friction = body->Friction();
  fixtureDef.restitution = body->Restitution();
  fixtureDef.filter.groupIndex = body->Group();
  fixtureDef.filter.maskBits = body->Mask();
  fixtureDef.filter.categoryBits = body->Category();

  b2Shape* shape;
  if (!body->Shape()) {
    LOG_WARNING << "Failed to add PhysicsBodyComponent: "
                        << "No CollisionShape attached!" << std::endl;
    shape = new b2CircleShape();
  } else {
    shape = body->Shape()->get_shape(transform);
  }
  fixtureDef.shape = shape;
  result->CreateFixture(&fixtureDef);

  result->SetBullet(body->IsBullet());
  result->SetFixedRotation(body->FixedRotation());
  result->SetLinearDamping(body->LinearDamping());
  result->SetAngularDamping(body->AngularDamping());
  result->SetActive(body->Enabled());
  result->SetUserData(body);

  delete shape;

  return result;
}

////////////////////////////////////////////////////////////////////////////////

void Physics::add(GravitySourceComponent* source) {
  gravity_sources_.insert(source);
}

////////////////////////////////////////////////////////////////////////////////

void Physics::add_shock_wave(math::uint64 damage_source, math::vec2 const& location, float damage, float radius, float strength) {
  ShockWave s;
  s.damage_source = damage_source;
  s.location = location;
  s.damage = damage;
  s.radius = radius;
  s.strength = strength;
  shock_waves_.push_back(s);
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

bool Physics::ray_cast(math::vec2 const& start, math::vec2 const& end,
                       std::vector<PhysicsBodyComponent*>& hits,
                       std::vector<math::vec2>& hit_points,
                       std::vector<math::vec2>& hit_normals) const {

  SwiftRayCastCallback callback;
  world_->RayCast(&callback, b2Vec2(start.x(), start.y()), b2Vec2(end.x(), end.y()));
  hits = callback.hits;
  hit_points = callback.hit_points;
  hit_normals = callback.hit_points;

  return !hit_points.empty();
}

////////////////////////////////////////////////////////////////////////////////

void Physics::update_gravity_map(ConstSerializedScenePtr const& scene, RenderContext const& ctx) {
  gravity_map_->process(scene, ctx);
}

////////////////////////////////////////////////////////////////////////////////

}
