////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/settings/SettingsWrapper.hpp>

#include <swift2d/objects/SavableObjectVisitor.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::set_settings_type(std::string const& type) {
  settings_type_ = type;
  load();
}

////////////////////////////////////////////////////////////////////////////////

SettingsWrapper::SettingsWrapper()
  : settings_type_("EngineSettings") {
  load();
}

////////////////////////////////////////////////////////////////////////////////

SettingsWrapper::~SettingsWrapper() {
  save();
}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::load() {
  auto d(EngineSettings::create_from_file("settings.json"));
  if (d) {
    Settings = std::dynamic_pointer_cast<EngineSettings>(d);
  } else {
    Settings = std::dynamic_pointer_cast<EngineSettings>(Object::create(settings_type_));
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::save() {
  Settings->save_to_file("settings.json");
}

////////////////////////////////////////////////////////////////////////////////

}
