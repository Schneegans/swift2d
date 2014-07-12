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

Physics::Physics()
  : world_(new b2World(b2Vec2(0.f, 0.f))) {}

////////////////////////////////////////////////////////////////////////////////

Physics::~Physics() {
  delete world_;
}

////////////////////////////////////////////////////////////////////////////////

void Physics::update(double time) {

  auto body(world_->GetBodyList());

  while (body) {
    auto body_pos = body->GetWorldCenter();

    for (auto source: gravity_sources_) {
      auto transform(source->get_user()->WorldTransform());
      auto pos(math::get_translation(transform));
      auto scale(math::get_scale(transform));
      float mass(source->Density() * scale.x() * scale.y());
      b2Vec2 dist(b2Vec2(pos.x(), pos.y()) - body_pos);
      dist *= ((mass * body->GetMass()) / dist.LengthSquared());
      body->ApplyForceToCenter(dist, true);
    }

    body = body->GetNext();
  }

  world_->Step(time, 6, 2);
}

////////////////////////////////////////////////////////////////////////////////

b2Body* Physics::add(DynamicBodyComponent const* body) {
  auto transform(body->get_user()->WorldTransform());
  math::vec2 pos(math::get_translation(transform));
  float rot(math::get_rotation(transform));
  float scale(math::get_scale(transform).x());

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos.x(), pos.y());
  bodyDef.angle = rot;
  b2Body* result = world_->CreateBody(&bodyDef);

  b2CircleShape shape;
  shape.m_radius = body->Radius()*scale;
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.4f;
  fixtureDef.restitution = 0.4f;
  result->CreateFixture(&fixtureDef);

  result->SetLinearDamping(0.5);
  result->SetAngularDamping(5);

  return result;
}

////////////////////////////////////////////////////////////////////////////////

b2Body* Physics::add(StaticBodyComponent const* body) {
  auto transform(body->get_user()->WorldTransform());
  math::vec2 pos(math::get_translation(transform));
  float rot(math::get_rotation(transform));
  float scale(math::get_scale(transform).x());

  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(pos.x(), pos.y());
  bodyDef.angle = rot;
  b2Body* result = world_->CreateBody(&bodyDef);

  b2CircleShape shape;
  shape.m_radius = body->Radius()*scale;
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtureDef.friction = 0.4f;
  fixtureDef.restitution = 0.4f;
  result->CreateFixture(&fixtureDef);

  return result;
}

////////////////////////////////////////////////////////////////////////////////

void Physics::add(GravitySourceComponent const* source) {
  gravity_sources_.insert(source);
}

////////////////////////////////////////////////////////////////////////////////

}
