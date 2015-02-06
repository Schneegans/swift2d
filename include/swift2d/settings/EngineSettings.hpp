////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_ENGINE_SETTINGS_HPP
#define SWIFT2D_ENGINE_SETTINGS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/objects/SavableObject.hpp>
#include <swift2d/properties.hpp>
#include <swift2d/graphics/Window.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class EngineSettings;
typedef std::shared_ptr<EngineSettings>       EngineSettingsPtr;
typedef std::shared_ptr<const EngineSettings> ConstEngineSettingsPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL EngineSettings : public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  Bool  VSync;

  Property<Window::Mode> WindowMode;
  Vec2ui WindowSize;

  Bool   SubSampling;
  Bool   DynamicLighting;
  Bool   LightSubSampling;
  Bool   LensFlares;
  Bool   HeatEffect;
  Float  Gamma;

  // ----------------------------------------------------- contruction interface
  template <typename... Args>
  static EngineSettingsPtr create(Args&& ... a) {
    return std::make_shared<EngineSettings>(a...);
  }

  EngineSettings();

  void accept(SavableObjectVisitor& to);

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "EngineSettings"; }
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_ENGINE_SETTINGS_HPP
