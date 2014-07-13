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
  : body_(nullptr) {}

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

}
