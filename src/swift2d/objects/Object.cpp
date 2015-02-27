////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/objects/Object.hpp>

#include <iostream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

namespace {
  std::unordered_map<std::string, std::function<ObjectPtr()>> factory;
}

////////////////////////////////////////////////////////////////////////////////

ObjectPtr Object::create(std::string const& type_name) {
  auto func(factory.find(type_name));
  if (func != factory.end()) {
    return (func->second)();
  }
  LOG_WARNING << "Failed to create Object of type \"" << type_name.c_str()
              << "\": Type not registered!" << std::endl;
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void Object::register_type(std::string const& name, std::function<ObjectPtr()> const& func) {
  factory[name] = func;
}

////////////////////////////////////////////////////////////////////////////////

}
