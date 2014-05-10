////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/Group.hpp>

// swift2d headers
#include <swift2d/scene/Node.hpp>
#include <swift2d/scene/TraversalStateStack.hpp>

// external headers
#include <algorithm>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////

/* virtual */
CorePtr const& Group::create_copy() const {
  if (!copy_) {
    auto copy(std::make_shared<Group>());
    copy->set_enabled(is_enabled());
    copy->children_.reserve(children_.size());

    for(auto const& child: children_) {
      copy->add_node(child->create_copy());
    }

    copy_ = copy;
  }
  return copy_;
}

////////////////////////////////////////////////////////////////////////////////

/* virtual */
void Group::clear_copy() const {
  Core::clear_copy();

  for(auto const& child: children_) {
    child->clear_copy();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Group::add_node(NodePtr const& child) {
  GroupInterface::add_node(child);
  children_.push_back(child);
}

////////////////////////////////////////////////////////////////////////////////

void Group::remove_node(NodePtr const& child) {
  GroupInterface::remove_node(child);
  children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
}
////////////////////////////////////////////////////////////////////////////////

void Group::traverse(Node const& parent,
                     TraversalStateStack& state,
                     NodePath& path) const {

  for(auto const& child: children_) {
    child->traverse(state, path);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Group::serialize(Node const& parent,
                      TraversalStateStack& state,
                      SerializedScene& scene) const {

  for(auto const& child: children_) {
    child->serialize(state, scene);
  }
}

////////////////////////////////////////////////////////////////////////////////

}}
