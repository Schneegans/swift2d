////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_CORE_HPP
#define SWIFT2D_CORE_HPP

// includes  -------------------------------------------------------------------
#include <vector>
#include <memory>

namespace swift {

// forward declares ------------------------------------------------------------
class Node;
class TraversalStateStack;
class NodePath;
class SerializedScene;

typedef std::shared_ptr<Node>       NodePtr;
typedef std::shared_ptr<const Node> ConstNodePtr;

namespace cores {

////////////////////////////////////////////////////////////////////////////////
// Nodes are made of multiple Cores. A core encapsulates fine-grained         //
// functionality, data or behaviors. During scene traversal and serialization //
// each core may influence the result and may pass information to subsequent  //
// cores.                                                                     //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Core;
typedef std::shared_ptr<Core> CorePtr;

// -----------------------------------------------------------------------------
class Core {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // --------------------------------------------------- configuration interface

  // Derived cores may implement a Config struct derived from this struct. Add
  // members to this config with the SWIFT2D_ADD_CONFIG_PROPERTY macro.
  struct Config {
    virtual ~Config() {}
  };
  virtual Config& config() { return config_;};

  // ------------------------------------------------------- traversal interface

  // Derived cores may implement a specific TraversalState which is passed to
  // subsequent cores during scene Traversal.
  struct TraversalState {
    virtual ~TraversalState() {}
  };

  // Derived cores may implement a specific SerializedState which is stored in a
  // SerializedScene and will be passed to the renderer.
  struct SerializedState {
    virtual ~SerializedState() {}
  };

  // ----------------------------------------------------- contruction interface

  // Creates a new core and returns a shared pointer.
  Core() : enabled_(true) {}
  virtual ~Core() {}

  // Performs a deep copy.
  CorePtr deep_copy() const {
    auto result(create_copy());
    clear_copy();
    return result;
  }

  // ------------------------------------------------------------ public methods

  // True, if this core should be processed in the serialization and traversal.
  bool is_enabled() const {return enabled_;}
  void set_enabled (bool enabled) {enabled_ = enabled;}

  // Returns all users of this core.
  std::vector<Node*> const& get_users() const {return users_;}

  // Node is friend, as it calls traverse and serialze methods.
  friend class ::swift::Node;

 ///////////////////////////////////////////////////////////////////////////////
 // -------------------------------------------------------- protected interface
 protected:

  // Derived class should implement any functionality which affects traversal to
  // a specified node at the location given by the NodePath.
  virtual void traverse(
    Node const& user,
    TraversalStateStack& state,
    NodePath& path) const {};

  // Derived class should implement this function if they want to affect the
  // serialized scene which is passed to the renderer.
  virtual void serialize(
    Node const& user,
    TraversalStateStack& state,
    SerializedScene& serialized) const {};

  // Performs a deep copy.
  virtual CorePtr const& create_copy() const = 0;

  // Clears the copy-cache. To avoid creation of multiple copies due to mutli-
  // usage of this core, create_copy() will initialize the copy-cache until it
  // gets cleared with this method.
  virtual void clear_copy() const { copy_ = nullptr; }

  // A pointer to the copy-cache.
  mutable CorePtr copy_;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  // adding / removal of nodes using this core
  void add_user(Node* new_user);
  void remove_user(Node* user);

  // holds a pointer to all users of this core
  std::vector<Node*> users_;

  // holds configuration information
  Config config_;

  // true, if this core should be processed in the serialization and traversal
  bool enabled_;
};

}}

#endif  // SWIFT2D_CORE_HPP
