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
}

////////////////////////////////////////////////////////////////////////////////

SettingsWrapper::SettingsWrapper()
  : settings_type_("EngineSettings") {
}

////////////////////////////////////////////////////////////////////////////////

SettingsWrapper::~SettingsWrapper() {}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::load_from_file(std::string const& file_name) {
  auto d(EngineSettings::create_from_file(file_name));
  if (d) {
    Settings = std::dynamic_pointer_cast<EngineSettings>(d);
  } else {
    Settings = std::dynamic_pointer_cast<EngineSettings>(Object::create(settings_type_));
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::save_to_file(std::string const& file_name) {
  Settings->save_to_file(file_name);
}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::load_from_steam_file(std::string const& file_name) {
  auto d(EngineSettings::create_from_file(file_name));
  if (d) {
    Settings = std::dynamic_pointer_cast<EngineSettings>(d);
  } else {
    Settings = std::dynamic_pointer_cast<EngineSettings>(Object::create(settings_type_));
  }
}

////////////////////////////////////////////////////////////////////////////////

void SettingsWrapper::save_to_steam_file(std::string const& file_name) {
  Settings->save_to_file(file_name);
}


////////////////////////////////////////////////////////////////////////////////

}
