////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_NODE_PATH_HPP
#define SWIFT2D_NODE_PATH_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/TraversalStateStack.hpp>
#include <vector>
#include <memory>

namespace swift {

// forward declares ------------------------------------------------------------
class Node;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class NodePath {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  NodePath()
    : nodes_() {}

  NodePath(std::vector<Node const*> const& nodes)
    : nodes_(nodes) {}

  NodePath(NodePath const& other)
    : nodes_(other.nodes_) {}

  void push_node(Node const* node) { nodes_.push_back(node); }

  Node const* front() const;
  Node const* back() const;

  bool     empty()  const { return nodes_.empty(); }
  unsigned length() const { return nodes_.size(); }

  NodePath sub_path(unsigned begin, unsigned end) const;

  TraversalStateMap traverse();

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  std::vector<Node const*> nodes_;

};

}

#endif  // SWIFT2D_NODE_PATH_HPP
