////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/Sprite.hpp>

// swift2d headers
#include <swift2d/scene/TraversalStateStack.hpp>
#include <swift2d/scene/Transformation.hpp>
#include <swift2d/serializer/SerializedScene.hpp>
#include <swift2d/utils/Logger.hpp>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////

/* virtual */
CorePtr const& Sprite::create_copy() const {
  if (!copy_) {
    copy_ = std::make_shared<Sprite>(config_);
    copy_->set_enabled(is_enabled());
  }
  return copy_;
}

////////////////////////////////////////////////////////////////////////////////

void Sprite::serialize(
  Node const& parent,
  TraversalStateStack& state,
  SerializedScene& serialized) const {

  glm::mat4 transform(glm::mat4(1.0f));

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

}}
