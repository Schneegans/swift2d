////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/settings/Settings.hpp>

#include <swift2d/objects/SavableObjectVisitor.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Settings::Settings() {
  load();
}

////////////////////////////////////////////////////////////////////////////////

Settings::~Settings() {
  save();
}

////////////////////////////////////////////////////////////////////////////////

void Settings::load() {
  auto d(DisplaySettings::create_from_file("display_settings.json"));
  if (d) {
    Display = *std::dynamic_pointer_cast<DisplaySettings>(d);
  }
}

////////////////////////////////////////////////////////////////////////////////

void Settings::save() {
  Display.save_to_file("display_settings.json");
}

////////////////////////////////////////////////////////////////////////////////

}
