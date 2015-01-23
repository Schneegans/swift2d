////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/components/RespawnComponent.hpp>

#include <swift2d/components/LifeComponent.hpp>
#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

RespawnComponent::RespawnComponent()
  : RespawnTime(10)
  , initialized_(false) {}

////////////////////////////////////////////////////////////////////////////////

void RespawnComponent::update(double time) {

  if (!initialized_) {
    initialized_ = true;

    auto life = get_user()->get_component<LifeComponent>();
    if (life) {
      life->on_killed.connect([this](math::uint64){
        scheduler_.execute_delayed(RespawnTime(), [this](){
          auto life = get_user()->get_component<LifeComponent>();
          if (life) {
            life->reset();
          }

          on_respawn.emit();
        });
        return true;
      });
    } else {
      LOG_WARNING << "Failed to initialize RespawnComponent: No LifeComponent found!" << std::endl;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void RespawnComponent::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("RespawnTime", RespawnTime);
}

////////////////////////////////////////////////////////////////////////////////

}