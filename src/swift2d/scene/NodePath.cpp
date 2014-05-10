////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/NodePath.hpp>

#include <swift2d/scene/Node.hpp>



namespace swift {

////////////////////////////////////////////////////////////////////////////////

Node const* NodePath::back() const {
  if (nodes_.empty()) {
    return nullptr;
  }
  return nodes_.back();
}

////////////////////////////////////////////////////////////////////////////////

Node const* NodePath::front() const {
  if (nodes_.empty()) {
    return nullptr;
  }
  return nodes_.front();
}

////////////////////////////////////////////////////////////////////////////////

NodePath NodePath::sub_path(unsigned begin, unsigned end) const {
  return NodePath(std::vector<Node const*>(nodes_.begin() + begin, nodes_.begin() + end));
}

////////////////////////////////////////////////////////////////////////////////

TraversalStateMap NodePath::traverse() {

  TraversalStateStack state;

  if (!empty()) {
    NodePath copy(*this);
    nodes_[0]->traverse(state, copy);
  }

  return state.get_all();
}

////////////////////////////////////////////////////////////////////////////////

}
