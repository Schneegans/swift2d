////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SERIALIZED_SCENE_HPP
#define SWIFT2D_SERIALIZED_SCENE_HPP

// swift2d headers
#include <swift2d/serializer/SerializedNode.hpp>

// external headers
#include <vector>
#include <string>
#include <set>

namespace swift {

struct SerializedScene {

  // All used materials.
  std::set<std::string> materials_;

  std::unordered_map<
    std::type_index,
    std::vector<cores::Core::SerializedState*>> cores;

};

}

#endif  // SWIFT2D_SERIALIZED_SCENE_HPP
