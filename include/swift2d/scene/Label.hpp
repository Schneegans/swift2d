////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_LABEL_CORE_HPP
#define SWIFT2D_LABEL_CORE_HPP

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
class Label;
typedef std::shared_ptr<Label>        LabelPtr;
typedef std::shared_ptr<const Label>  ConstLabelPtr;

// -----------------------------------------------------------------------------
class Label : public Core {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // --------------------------------------------------- configuration interface

  // The config struct contains a public member for each macro called inside.
  // There is also a set_* methos for each member returning a reference to the
  // config struct itself, which allows for chaining setters.
  struct Config: public Core::Config {
    Config() {}

    SWIFT2D_ADD_CONFIG_PROPERTY(String, name, "");
  };

  Config & config() { return config_; }

  // ----------------------------------------------------- contruction interface

  // Creates a new core and returns a shared pointer.
  static LabelPtr create(Config const& config = Config()) {
    return std::make_shared<Label>(config);
  }

  Label(Config const& config = Config()) : config_(config) {}

  // Performs a deep copy.
  /* virtual */ CorePtr const& create_copy() const;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  Config config_;
};

}}

#endif  // SWIFT2D_LABEL_CORE_HPP
