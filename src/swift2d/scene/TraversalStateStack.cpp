////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/TraversalStateStack.hpp>

#include <swift2d/scene/Core.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

void TraversalStateStack::save() {
  if (state_.empty()) {
    state_.push(TraversalStateMap());
  } else {
    state_.push(state_.top());
  }
}

////////////////////////////////////////////////////////////////////////////////

void TraversalStateStack::restore() {
  if (!state_.empty()) {
    state_.pop();
  }
}

}
