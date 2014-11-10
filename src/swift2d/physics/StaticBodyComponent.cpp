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
  : Density(1.0f)
  , Friction(0.5f)
  , Restitution(0.5f)
  , Group(0)
  , Mask(-1)
  , Category(0)
  , body_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

StaticBodyComponent::~StaticBodyComponent() {
  if (body_) {
    Physics::get().remove(body_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void StaticBodyComponent::update(double time) {
  if (!body_) {
    body_ = Physics::get().add(this);

    body_->SetActive(Enabled());
    Enabled.on_change().connect([this](bool enabled) {
      if (body_) {
        body_->SetActive(enabled);
      }
      return true;
    });

    Shape.on_change().connect([&](CollisionShapePtr const&){
      LOG_WARNING << "Updating collision shapes is not implmented yet!"
                          << std::endl;
      return true;
    });
    Density.on_change().connect([&](float val){
      body_->GetFixtureList()->SetDensity(val);
      body_->ResetMassData();
      return true;
    });
    Friction.on_change().connect([&](float val){
      body_->GetFixtureList()->SetFriction(val);
      return true;
    });
    Restitution.on_change().connect([&](float val){
      body_->GetFixtureList()->SetRestitution(val);
      return true;
    });
    Group.on_change().connect([&](math::int16 val){
      auto f(body_->GetFixtureList()->GetFilterData());
      f.groupIndex = val;
      body_->GetFixtureList()->SetFilterData(f);
      return true;
    });
    Mask.on_change().connect([&](math::uint16 val){
      auto f(body_->GetFixtureList()->GetFilterData());
      f.maskBits = val;
      body_->GetFixtureList()->SetFilterData(f);
      return true;
    });
    Category.on_change().connect([&](math::uint16 val){
      auto f(body_->GetFixtureList()->GetFilterData());
      f.categoryBits = val;
      body_->GetFixtureList()->SetFilterData(f);
      return true;
    });
  }
}

////////////////////////////////////////////////////////////////////////////////

void StaticBodyComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_object_property("Shape", Shape);
  visitor.add_member("Density", Density);
  visitor.add_member("Friction", Friction);
  visitor.add_member("Restitution", Restitution);
  visitor.add_member("Group", Group);
  visitor.add_member("Mask", Mask);
  visitor.add_member("Category", Category);
}

////////////////////////////////////////////////////////////////////////////////

}
