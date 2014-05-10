////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_MATERIAL_CORE_HPP
#define SWIFT2D_MATERIAL_CORE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Core.hpp>
#include <swift2d/utils/configuration_macro.hpp>
#include <swift2d/events.hpp>

#include <string>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Material;
typedef std::shared_ptr<Material>       MaterialPtr;
typedef std::shared_ptr<const Material> ConstMaterialPtr;

// -----------------------------------------------------------------------------
class Material : public Core {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // --------------------------------------------------- configuration interface

  // The config struct contains a public member for each macro called inside.
  // There is also a set_* methos for each member returning a reference to the
  // config struct itself, which allows for chaining setters.
  struct Config: public Core::Config {
    Config() {}

    SWIFT2D_ADD_CONFIG_PROPERTY(String, material, "");
  };

  Config & config() { return config_; }

  // ------------------------------------------------------- traversal interface

  struct TraversalState : public Core::TraversalState {
    std::string material;
  };


  // ----------------------------------------------------- contruction interface

  // Creates a new core and returns a shared pointer.
  static MaterialPtr create(Config const& config = Config()) {
    return std::make_shared<Material>(config);
  }

  Material(Config const& config = Config()) : config_(config) {}

  // Performs a deep copy.
  /* virtual */ CorePtr const& create_copy() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  /* virtual */ void serialize(
    Node const& parent,
    TraversalStateStack& state,
    SerializedScene& scene) const;

  Config config_;

};

}}

#endif  // SWIFT2D_MATERIAL_CORE_HPP
