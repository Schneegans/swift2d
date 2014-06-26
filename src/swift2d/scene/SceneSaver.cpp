////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/scene/SceneSaver.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void SceneSaver::save(std::string const& name, SerializableReference const& value) {
  saved_members_.push_back(std::make_pair(name, value));
}

////////////////////////////////////////////////////////////////////////////////

boost::property_tree::ptree SceneSaver::to_json(std::string const& type_name) {
  boost::property_tree::ptree tree;
  tree.put("Type", type_name);

  for (auto& member: saved_members_) {
    member.second.serialize(member.first, tree);
  }

  return tree;
}

////////////////////////////////////////////////////////////////////////////////

}
