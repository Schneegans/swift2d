////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SETTINGS_HPP
#define SWIFT2D_SETTINGS_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/settings/DisplaySettings.hpp>
#include <swift2d/utils/Singleton.hpp>
#include <swift2d/objects/SavableObject.hpp>

namespace swift {

// -----------------------------------------------------------------------------
class Settings: public Singleton<Settings> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  DisplaySettings& display();

  friend class Singleton<Settings>;

 private:
  void load();
  void save();

  Settings();
  ~Settings();

  DisplaySettings display_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SETTINGS_HPP
