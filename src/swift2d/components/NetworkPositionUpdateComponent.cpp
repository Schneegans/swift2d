////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/components/NetworkPositionUpdateComponent.hpp>

#include <swift2d/components/LifeComponent.hpp>
#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

NetworkPositionUpdateComponent::NetworkPositionUpdateComponent()
  : update_interval_(0.1)
  , time_(update_interval_)
  , is_local_(true) {}

////////////////////////////////////////////////////////////////////////////////

void NetworkPositionUpdateComponent::init(bool is_local) {
  is_local_ = is_local;
  body_ = get_user()->get_component<DynamicBodyComponent>();

  if (!body_) {
    LOG_WARNING << "Failed to initialize NetworkPositionUpdateComponent: No DynamicBodyComponent found!" << std::endl;
  }

  if (!is_local_) {
    PosRotUpdate.on_change().connect([this](math::vec3 const& val) {
      if (body_) {
        body_->set_transform(val.xy(), val.z());
      }
      return true;
    });

    LinAngUpdate.on_change().connect([this](math::vec3 const& val) {
      if (body_) {
        body_->set_linear_velocity(val.xy());
        body_->set_angular_velocity(val.z());
      }
      return true;
    });
  } else {
    send_update();
  }
}

////////////////////////////////////////////////////////////////////////////////

void NetworkPositionUpdateComponent::update(double time) {
  if (is_local_) {
    time_ += time;
    if (time_ >= update_interval_) {
      time_ = 0.f;
      send_update();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void NetworkPositionUpdateComponent::send_update() {
  if (body_) {
    auto pos(math::get_translation(get_user()->Transform.get()));
    auto rot(math::get_rotation(get_user()->Transform.get()));
    PosRotUpdate.set(math::vec3(pos.x(), pos.y(), rot));

    auto lin_vel(body_->get_linear_velocity());
    auto ang_vel(body_->get_angular_velocity());
    LinAngUpdate.set(math::vec3(lin_vel.x(), lin_vel.y(), ang_vel));
  }
}

////////////////////////////////////////////////////////////////////////////////

void NetworkPositionUpdateComponent::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("UpdateInterval", update_interval_);
}

////////////////////////////////////////////////////////////////////////////////

}
