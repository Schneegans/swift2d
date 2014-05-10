////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/PointLight.hpp>

// swift2d headers
#include <swift2d/scene/TraversalStateStack.hpp>
#include <swift2d/scene/Transformation.hpp>
#include <swift2d/serializer/SerializedScene.hpp>

namespace swift {

namespace cores {

////////////////////////////////////////////////////////////////////////////////

/* virtual */
CorePtr const& PointLight::create_copy() const {
  if (!copy_) {
    copy_ = std::make_shared<PointLight>(config_);
    copy_->set_enabled(is_enabled());
  }
  return copy_;
}

////////////////////////////////////////////////////////////////////////////////

void PointLight::serialize(
  Node const& parent,
  TraversalStateStack& state,
  SerializedScene& serialized) const {

  // TODO: Culling

  math::mat3 transform;

  auto tranform_state = state.get<Transformation::TraversalState>();
  if (tranform_state) {
    transform = tranform_state->world_transform;
  }

  auto s = new SerializedState();
  s->config = config_;
  s->transform = transform;
  serialized.cores[typeid(SerializedState)].push_back(s);
}

////////////////////////////////////////////////////////////////////////////////

}
}
