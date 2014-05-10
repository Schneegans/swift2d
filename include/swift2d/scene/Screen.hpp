////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SCREEN_CORE_HPP
#define SWIFT2D_SCREEN_CORE_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Core.hpp>
#include <swift2d/utils/configuration_macro.hpp>
#include <swift2d/math.hpp>
#include <swift2d/events.hpp>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////
// A screen core represents a rectangular, virtual screen. Combined with a    //
// view core it defines the viewing frustum.                                  //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Screen;
typedef std::shared_ptr<Screen>       ScreenPtr;
typedef std::shared_ptr<const Screen> ConstScreenPtr;

// -----------------------------------------------------------------------------
class Screen : public Core {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // --------------------------------------------------- configuration interface

  // The config struct contains a public member for each macro called inside.
  // There is also a set_* methos for each member returning a reference to the
  // config struct itself, which allows for chaining setters.
  struct Config: public Core::Config {
    Config(){}

    // the world space size of the screen
    SWIFT2D_ADD_CONFIG_PROPERTY(Vec2, size, math::vec2(1.f, 1.f));
  };

  /* virtual */ Config& config() { return config_; }

  // ----------------------------------------------------- contruction interface

  // Creates a new core and returns a shared pointer.
  static ScreenPtr create(Config const& c = Config()) {
    return std::make_shared<Screen>(c);
  }

  // Creates a new core -- usually the create methos above will be used, since
  // almost everywhere shared pointer are used.
  Screen(Config const& c = Config()) : config_(c) {}

  // Performs a deep copy.
  /* virtual */ CorePtr const& create_copy() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:

  // holds configuration information
  Config config_;
};

// -----------------------------------------------------------------------------

}}

#endif  // SWIFT2D_SCREEN_CORE_HPP
