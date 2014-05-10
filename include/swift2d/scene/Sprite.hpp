////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_CORE_HPP
#define SWIFT2D_SPRITE_CORE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Core.hpp>
#include <swift2d/utils/configuration_macro.hpp>
#include <swift2d/math/math.hpp>
#include <swift2d/events.hpp>

#include <string>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Sprite;
typedef std::shared_ptr<Sprite>       SpritePtr;
typedef std::shared_ptr<const Sprite> ConstSpritePtr;

// -----------------------------------------------------------------------------
class Sprite : public Core {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // --------------------------------------------------- configuration interface

  // The config struct contains a public member for each macro called inside.
  // There is also a set_* methos for each member returning a reference to the
  // config struct itself, which allows for chaining setters.
  struct Config: public Core::Config {
    Config(){}

    SWIFT2D_ADD_CONFIG_PROPERTY(String, texture, "");
  };

  /* virtual */ Config& config() { return config_; }

  // ------------------------------------------------------- traversal interface

  struct SerializedState: public Core::SerializedState {
    Config config;
    glm::mat4 transform;
  };

  // ----------------------------------------------------- contruction interface

  // Creates a new core and returns a shared pointer.
  static SpritePtr create(Config const& c = Config()) {
    return std::make_shared<Sprite>(c);
  }

  Sprite(Config const& c = Config()) : config_(c) {}

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

#endif  // SWIFT2D_SPRITE_CORE_HPP
