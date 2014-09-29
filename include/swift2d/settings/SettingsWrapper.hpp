////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SETTINGS_WRAPPER_HPP
#define SWIFT2D_SETTINGS_WRAPPER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/settings/EngineSettings.hpp>
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/objects/SavableObject.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class SettingsWrapper: public Singleton<SettingsWrapper> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  EngineSettingsPtr Settings;

  template<typename T>
  std::shared_ptr<T> settings() {
    return std::dynamic_pointer_cast<T>(Settings);
  }

  void set_settings_type(std::string const& type);

  friend class Singleton<SettingsWrapper>;

 private:
  void load();
  void save();

  SettingsWrapper();
  ~SettingsWrapper();

  std::string settings_type_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SETTINGS_WRAPPER_HPP