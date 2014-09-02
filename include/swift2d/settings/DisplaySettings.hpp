////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_DISPLAY_SETTINGS_HPP
#define SWIFT2D_DISPLAY_SETTINGS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/objects/SavableObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class DisplaySettings;
typedef std::shared_ptr<DisplaySettings>       DisplaySettingsPtr;
typedef std::shared_ptr<const DisplaySettings> ConstDisplaySettingsPtr;

// -----------------------------------------------------------------------------
class DisplaySettings: public SavableObject {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:
  Bool  VSync;
  Bool  Fullscreen;

  // 0: no eye candy
  // 1: lighting
  // 2: blur + lens flares
  // 3: heat
  Int   ShadingQuality;
  Bool  SubSampling;
  Float Gamma;

  // ----------------------------------------------------- contruction interface
  template <typename... Args>
  static DisplaySettingsPtr create(Args&& ... a) {
    return std::make_shared<DisplaySettings>(a...);
  }

  DisplaySettings();

  void accept(SavableObjectVisitor& to);

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "DisplaySettings"; }
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_DISPLAY_SETTINGS_HPP
