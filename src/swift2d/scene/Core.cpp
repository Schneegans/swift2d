////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/scene/Core.hpp>

#include <algorithm>

namespace swift {
namespace cores {

////////////////////////////////////////////////////////////////////////////////

void Core::add_user(Node* new_user) {
  users_.push_back(new_user);
}

////////////////////////////////////////////////////////////////////////////////

void Core::remove_user(Node* user) {
  users_.erase(std::remove(users_.begin(), users_.end(), user), users_.end());
}

////////////////////////////////////////////////////////////////////////////////

}}
