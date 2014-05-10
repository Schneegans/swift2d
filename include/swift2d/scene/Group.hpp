////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_GROUP_CORE_HPP
#define SWIFT2D_GROUP_CORE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/GroupInterface.hpp>
#include <vector>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Group;
typedef std::shared_ptr<Group>        GroupPtr;
typedef std::shared_ptr<const Group>  ConstGroupPtr;

// -----------------------------------------------------------------------------
class Group : public GroupInterface {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface

  // Creates a new core and returns a shared pointer.
  static GroupPtr create() {
    return std::make_shared<Group>();
  }

  // Performs a deep copy.
  /* virtual */ CorePtr const& create_copy() const;
  /* virtual */ void clear_copy() const;

  /* virtual */ void add_node(NodePtr const& child);
  /* virtual */ void remove_node(NodePtr const& child);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  /* virtual */ void traverse(Node const& parent,
                              TraversalStateStack& state,
                              NodePath& path) const;
  /* virtual */ void serialize(Node const& parent,
                               TraversalStateStack& state,
                               SerializedScene& scene) const;

  std::vector<NodePtr> children_;

};

}}

#endif  // SWIFT2D_GROUP_CORE_HPP
