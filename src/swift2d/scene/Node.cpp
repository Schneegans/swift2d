////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/Node.hpp>

// swift2d headers
#include <swift2d/scene/TraversalStateStack.hpp>
#include <swift2d/scene/GroupInterface.hpp>

#include <iostream>
#include <algorithm>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

NodePtr Node::deep_copy() const {
  auto result(create_copy());
  clear_copy();

  return result;
}

////////////////////////////////////////////////////////////////////////////////

void Node::remove_core(cores::CorePtr const& core) {
  auto delete_pos(std::remove(cores_.begin(), cores_.end(), core));

  if (delete_pos != cores_.end()) {
    (*delete_pos)->remove_user(this);
    cores_.erase(delete_pos, cores_.end());
  }
}

////////////////////////////////////////////////////////////////////////////////

std::vector<NodePath> Node::get_paths() const {
  if (parents_.empty()) {
    auto paths(std::vector<NodePath>(1));
    paths[0].push_node(this);
    return paths;
  }

  if (parents_.size() == 1 && parents_[0]->get_users().size() == 1 ) {
    auto parent_paths = parents_[0]->get_users()[0]->get_paths();
    for (auto& path : parent_paths) {
      path.push_node(this);
    }
    return parent_paths;
  }

  std::vector<NodePath> paths;
  for (auto parent_core : parents_) {
    for (auto parent_user : parent_core->get_users()) {
      auto parent_paths = parent_user->get_paths();
      for (auto& path : parent_paths) {
        path.push_node(this);
      }
      paths.insert(paths.end(), parent_paths.begin(), parent_paths.end());
    }
  }
  return paths;
}


////////////////////////////////////////////////////////////////////////////////

void Node::traverse(TraversalStateStack& state, NodePath& path) const {
  if (!path.empty() && path.front() == this) {

    path = path.sub_path(1, path.length());

    bool final_node(path.empty());

    state.save();

    for (auto const& core: cores_) {
      if (!path.empty() || final_node) {
        core->traverse(*this, state, path);
      } else {
        break;
      }
    }
  }

}

////////////////////////////////////////////////////////////////////////////////

void Node::serialize(TraversalStateStack& state, SerializedScene& scene) const {
  state.save();
  for (auto const& core: cores_) {
    core->serialize(*this, state, scene);
  }
  state.restore();
}

////////////////////////////////////////////////////////////////////////////////

void Node::add_parent(cores::GroupInterface* new_parent) {
  parents_.push_back(new_parent);
}

////////////////////////////////////////////////////////////////////////////////

void Node::remove_parent(cores::GroupInterface* parent) {
  parents_.erase(std::remove(parents_.begin(), parents_.end(), parent), parents_.end());
}

////////////////////////////////////////////////////////////////////////////////

NodePtr const& Node::create_copy() const {

  if (!copy_) {
    copy_ = std::make_shared<Node>();
    copy_->cores_.reserve(cores_.size());
    for (auto const& core: cores_) {
      copy_->add_core(core->create_copy());
    }
  }

  return copy_;
}

////////////////////////////////////////////////////////////////////////////////

void Node::clear_copy() const {
  copy_ = nullptr;
  for (auto const& core: cores_) {
    core->clear_copy();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
