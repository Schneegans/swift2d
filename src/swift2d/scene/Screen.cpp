////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/Screen.hpp>

// swift2d headers
#include <swift2d/scene/TraversalStateStack.hpp>
#include <swift2d/scene/Transformation.hpp>


namespace swift {

namespace cores {

////////////////////////////////////////////////////////////////////////////////

/* virtual */
CorePtr const& Screen::create_copy() const {
  if (!copy_) {
    copy_ = std::make_shared<Screen>(config_);
    copy_->set_enabled(is_enabled());
  }
  return copy_;
}

////////////////////////////////////////////////////////////////////////////////

}
}
