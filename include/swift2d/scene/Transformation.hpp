////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TRANSFORM_CORE_HPP
#define SWIFT2D_TRANSFORM_CORE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Core.hpp>
#include <swift2d/utils/configuration_macro.hpp>
#include <swift2d/math.hpp>
#include <swift2d/events.hpp>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Transformation;
typedef std::shared_ptr<Transformation>       TransformationPtr;
typedef std::shared_ptr<const Transformation> ConstTransformationPtr;

// -----------------------------------------------------------------------------
class Transformation : public Core {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:


  // --------------------------------------------------- configuration interface

  // The config struct contains a public member for each macro called inside.
  // There is also a set_* methos for each member returning a reference to the
  // config struct itself, which allows for chaining setters.
  struct Config: public Core::Config {
    Config() {}

    SWIFT2D_ADD_CONFIG_PROPERTY(Mat3, transform, math::mat3());
  };
  Config & config() { return config_; }

  // ------------------------------------------------------- traversal interface

  struct TraversalState : public Core::TraversalState {
    math::mat3 world_transform;
  };

  // ----------------------------------------------------- contruction interface

  // Creates a new core and returns a shared pointer.
  static TransformationPtr create(Config const& config = Config()) {
    return std::make_shared<Transformation>(config);
  }

  Transformation(Config const& config = Config()) : config_(config) {}

  // Performs a deep copy.
  /* virtual */ CorePtr const& create_copy() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  void update_state(TraversalStateStack& state) const;

  /* virtual */ void traverse(Node const& parent,
                              TraversalStateStack& state,
                              NodePath& path) const;

  /* virtual */ void serialize(Node const& parent,
                               TraversalStateStack& state,
                               SerializedScene& scene) const;

  Config config_;
};

}}

#endif  // SWIFT2D_TRANSFORM_CORE_HPP
