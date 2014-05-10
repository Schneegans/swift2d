////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/Transformation.hpp>

// swift2d headers
#include <swift2d/scene/TraversalStateStack.hpp>


namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////

/* virtual */
CorePtr const& Transformation::create_copy() const {
  if (!copy_) {
    copy_ = std::make_shared<Transformation>(config_);
    copy_->set_enabled(is_enabled());
  }
  return copy_;
}

////////////////////////////////////////////////////////////////////////////////

void Transformation::update_state(TraversalStateStack& state) const {
  auto my_state(std::make_shared<TraversalState>());
  my_state->world_transform = config_.transform.get();

  auto tranform_state = state.get<TraversalState>();
  if (tranform_state) {
    my_state->world_transform = tranform_state->world_transform * config_.transform.get();
  }

  state.set(my_state);
}

////////////////////////////////////////////////////////////////////////////////

void Transformation::traverse(Node const& parent,
                         TraversalStateStack& state,
                         NodePath& path) const {

  update_state(state);
}

////////////////////////////////////////////////////////////////////////////////

void Transformation::serialize(Node const& parent,
                          TraversalStateStack& state,
                          SerializedScene& scene) const {

  update_state(state);
}

////////////////////////////////////////////////////////////////////////////////

}}
