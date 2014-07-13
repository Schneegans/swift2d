////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/physics/StaticBodyComponent.hpp>

#include <swift2d/physics/Physics.hpp>
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/math/transformations.hpp>

#include <Box2D/Box2D.h>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

StaticBodyComponent::StaticBodyComponent()
  : Radius(1.0f)
  , Density(1.0f)
  , Friction(0.5f)
  , Restitution(0.5f)
  , body_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

StaticBodyComponent::~StaticBodyComponent() {
  if (body_) {
    Physics::instance()->remove(body_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void StaticBodyComponent::update(double time) {
  if (!body_) {
    body_ = Physics::instance()->add(this);

    Radius.on_change().connect([&](float val){
      body_->GetFixtureList()->GetShape()->m_radius = val;
      body_->ResetMassData();
    });
    Density.on_change().connect([&](float val){
      body_->GetFixtureList()->SetDensity(val);
      body_->ResetMassData();
    });
    Friction.on_change().connect([&](float val){
      body_->GetFixtureList()->SetFriction(val);
    });
    Restitution.on_change().connect([&](float val){
      body_->GetFixtureList()->SetRestitution(val);
    });
  }
}

////////////////////////////////////////////////////////////////////////////////

void StaticBodyComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("Radius", Radius);
  visitor.add_member("Density", Density);
  visitor.add_member("Friction", Friction);
  visitor.add_member("Restitution", Restitution);
}

////////////////////////////////////////////////////////////////////////////////

}
