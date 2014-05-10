////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_VIEW_CORE_HPP
#define SWIFT2D_VIEW_CORE_HPP

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
class View;
typedef std::shared_ptr<View>       ViewPtr;
typedef std::shared_ptr<const View> ConstViewPtr;

// -----------------------------------------------------------------------------
class View : public Core {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // --------------------------------------------------- configuration interface

  // The config struct contains a public member for each macro called inside.
  // There is also a set_* methos for each member returning a reference to the
  // config struct itself, which allows for chaining setters.
  struct Config: public Core::Config {
    Config() {}
    SWIFT2D_ADD_CONFIG_PROPERTY(Float, eye_distance, 0.0f);
  };

  Config & config() { return config_; }

  // ----------------------------------------------------- contruction interface

  // Creates a new core and returns a shared pointer.
  static ViewPtr create(Config const& config = Config()) {
    return std::make_shared<View>(config);
  }

  View(Config const& config = Config()) : config_(config) {}

  // Performs a deep copy.
  /* virtual */ CorePtr const& create_copy() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Config config_;
};

}}

#endif  // SWIFT2D_VIEW_CORE_HPP
