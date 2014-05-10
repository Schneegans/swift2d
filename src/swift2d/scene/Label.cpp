////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/Label.hpp>

// swift2d headers
#include <swift2d/scene/TraversalStateStack.hpp>


namespace swift {

namespace cores {

////////////////////////////////////////////////////////////////////////////////

/* virtual */
CorePtr const& Label::create_copy() const {
  if (!copy_) {
    copy_ = std::make_shared<Label>(config_);
    copy_->set_enabled(is_enabled());
  }
  return copy_;
}

////////////////////////////////////////////////////////////////////////////////

}
}
