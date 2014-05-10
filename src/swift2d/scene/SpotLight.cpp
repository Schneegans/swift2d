////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/SpotLight.hpp>

// swift2d headers
#include <swift2d/scene/TraversalStateStack.hpp>
#include <swift2d/scene/Transformation.hpp>
#include <swift2d/serializer/SerializedScene.hpp>

namespace swift {

namespace cores {

////////////////////////////////////////////////////////////////////////////////

/* virtual */
CorePtr const& SpotLight::create_copy() const {
  if (!copy_) {
    copy_ = std::make_shared<SpotLight>(config_);
    copy_->set_enabled(is_enabled());
  }
  return copy_;
}

////////////////////////////////////////////////////////////////////////////////

void SpotLight::serialize(
  Node const& parent,
  TraversalStateStack& state,
  SerializedScene& serialized) const {

  // TODO: Culling

  glm::mat4 transform(glm::mat4(1.f));

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
