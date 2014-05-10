////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/Material.hpp>

// swift2d headers
#include <swift2d/scene/TraversalStateStack.hpp>

// external headers
#include <memory>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////

/* virtual */
CorePtr const& Material::create_copy() const {
  if (!copy_) {
    copy_ = std::make_shared<Material>(config_);
    copy_->set_enabled(is_enabled());
  }
  return copy_;
}

////////////////////////////////////////////////////////////////////////////////

void Material::serialize(Node const& parent,
                         TraversalStateStack& state,
                         SerializedScene& scene) const {

  std::shared_ptr<TraversalState> my_state(new TraversalState());
  my_state->material = config_.material.get();
  state.set(my_state);
}

////////////////////////////////////////////////////////////////////////////////

}}
