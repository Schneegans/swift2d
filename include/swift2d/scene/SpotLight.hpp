////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPOT_LIGHT_CORE_HPP
#define SWIFT2D_SPOT_LIGHT_CORE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Core.hpp>
#include <swift2d/utils/configuration_macro.hpp>
#include <swift2d/utils/Color.hpp>
#include <swift2d/math/math.hpp>
#include <swift2d/events.hpp>
#include <string>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SpotLight;
typedef std::shared_ptr<SpotLight>        SpotLightPtr;
typedef std::shared_ptr<const SpotLight>  ConstSpotLightPtr;

// -----------------------------------------------------------------------------
class SpotLight : public Core {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:


  // --------------------------------------------------- configuration interface

  // The config struct contains a public member for each macro called inside.
  // There is also a set_* methos for each member returning a reference to the
  // config struct itself, which allows for chaining setters.
  struct Config: public Core::Config {
    Config(){}

    SWIFT2D_ADD_CONFIG_PROPERTY(ColorProperty, color, Color(1.f, 1.f, 1.f));
    SWIFT2D_ADD_CONFIG_PROPERTY(Float,   falloff,                  1.f);
    SWIFT2D_ADD_CONFIG_PROPERTY(Float,   softness,                 0.5f);
    SWIFT2D_ADD_CONFIG_PROPERTY(Bool,    enable_shadows,           false);
    SWIFT2D_ADD_CONFIG_PROPERTY(Bool,    enable_godrays,           false);
    SWIFT2D_ADD_CONFIG_PROPERTY(Bool,    enable_diffuse_shading,   true);
    SWIFT2D_ADD_CONFIG_PROPERTY(Bool,    enable_specular_shading,  true);
    SWIFT2D_ADD_CONFIG_PROPERTY(UInt,    shadow_map_size,          512);
    SWIFT2D_ADD_CONFIG_PROPERTY(Float,   shadow_offset,            0.001f);
  };

  Config & config() { return config_; }

  // ------------------------------------------------------- traversal interface

  struct SerializedState: public Core::SerializedState {
    Config config;
    glm::mat4 transform;
  };

  // ----------------------------------------------------- contruction interface

  // Creates a new core and returns a shared pointer.
  static SpotLightPtr create(Config const& config = Config()) {
    return std::make_shared<SpotLight>(config);
  }

  SpotLight(Config const& config = Config()) : config_(config) {}

  // Performs a deep copy.
  /* virtual */ CorePtr const& create_copy() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  /* virtual */ void serialize(
    Node const& parent,
    TraversalStateStack& state,
    SerializedScene& serialized) const;

  Config config_;

};

}}

#endif  // SWIFT2D_SPOT_LIGHT_CORE_HPP
