////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/components/DealDamageComponent.hpp>

#include <swift2d/components/LifeComponent.hpp>
#include <swift2d/scene/SceneObject.hpp>
#include <swift2d/physics/DynamicBodyComponent.hpp>
#include <swift2d/network/NetworkObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

DealDamageComponent::DealDamageComponent()
  : Amount(1)
  , DamageSourceID(0) {}

////////////////////////////////////////////////////////////////////////////////

void DealDamageComponent::on_init() {
  auto body(get_user()->get_component<DynamicBodyComponent>());
  if (body) {
    body->start_contact_with_dynamic.connect([this](DynamicBodyComponent* self, DynamicBodyComponent* other, math::vec2 const&) {
      auto net_object(dynamic_cast<NetworkObject*>(other->get_user()));
      if (!net_object || net_object->is_local()) {
        auto life = other->get_user()->get_component<LifeComponent>();
        if (life) {
          life->decrease(Amount(), DamageSourceID(), self->get_linear_velocity()*0.5);
        }
      }

      return true;
    });
  } else {
    LOG_WARNING << "Failed to initialize DealDamageComponent: No DynamicBodyComponent found!" << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void DealDamageComponent::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("Amount", Amount);
  visitor.add_member("DamageSourceID", DamageSourceID);
}

////////////////////////////////////////////////////////////////////////////////

}
