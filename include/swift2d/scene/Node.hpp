////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_NODE_HPP
#define SWIFT2D_NODE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Core.hpp>
#include <swift2d/scene/NodePath.hpp>

#include <vector>
#include <memory>

namespace swift {

// forward declares ------------------------------------------------------------
namespace cores {
  class GroupInterface;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Node;
typedef std::shared_ptr<Node>       NodePtr;
typedef std::shared_ptr<const Node> ConstNodePtr;

// -----------------------------------------------------------------------------
class Node {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  static NodePtr create() {
    return std::make_shared<Node>();
  }

  NodePtr deep_copy() const;
  NodePtr const& create_copy() const;
  void                         clear_copy()  const;

  template<typename T>
  typename std::shared_ptr<T> add_core(std::shared_ptr<T> const& core, int index = -1) {
    core->add_user(this);

    if (index < 0 || index >= cores_.size()) {
      cores_.push_back(core);
    } else {
      cores_.insert(cores_.begin() + index, core);
    }

    return core;
  }

  template<typename T>
  typename std::shared_ptr<T> add_core(int index = -1) {
    auto core = std::make_shared<T>();
    return add_core(core, index);
  }


  void remove_core(cores::CorePtr const& core);

  std::vector<cores::GroupInterface*> const& get_parents() const {return parents_;}
  std::vector<NodePath> get_paths() const;

  template<typename T>
  std::vector<std::shared_ptr<T>> get_cores() const {
    std::vector<std::shared_ptr<T>> result;

    for (auto const& core: cores_) {
      auto casted(std::dynamic_pointer_cast<T>(core));
      if (casted) {
        result.push_back(casted);
      }
    }
    return result;
  }

  template<typename T>
  std::shared_ptr<T> get_core() const {
    for (auto const& core: cores_) {
      auto casted(std::dynamic_pointer_cast<T>(core));
      if (casted) {
        return casted;
      }
    }
    return std::shared_ptr<T>();
  }

  template<typename T>
  bool has_core() const {
    return get_core<T>() != nullptr;
  }

  void traverse(TraversalStateStack& state, NodePath& path) const;
  void serialize(TraversalStateStack& state, SerializedScene& scene) const;

  friend class cores::GroupInterface;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void add_parent(cores::GroupInterface* new_parent);
  void remove_parent(cores::GroupInterface* parent);

  std::vector<cores::CorePtr> cores_;

  std::vector<cores::GroupInterface*> parents_;

  mutable NodePtr copy_;
};

}

#endif  // SWIFT2D_NODE_HPP
