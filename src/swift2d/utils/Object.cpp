////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/utils/Object.hpp>

#include <iostream>

namespace swift {

std::unordered_map<std::string, std::function<ObjectPtr()>> Object::factory_;

ObjectPtr Object::create(std::string const& type_name) {
  auto factory(factory_.find(type_name));
  if (factory != factory_.end()) {
    return (factory->second)();
  }
  Logger::LOG_WARNING << "Failed to create Object of type \"" << type_name
                      << "\": Type not registered!" << std::endl;
  return nullptr;
}

}
