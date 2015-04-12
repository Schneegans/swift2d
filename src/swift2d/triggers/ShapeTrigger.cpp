////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/triggers/ShapeTrigger.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ShapeTrigger::~ShapeTrigger() {
  if (a_) a_->WorldTransform.on_change().disconnect(a_callback_);
  if (b_) b_->WorldTransform.on_change().disconnect(b_callback_);
}

////////////////////////////////////////////////////////////////////////////////

void ShapeTrigger::set_shapes(CircularShapePtr const& a, CircularShapePtr const& b) {

  if (a_) a_->WorldTransform.on_change().disconnect(a_callback_);
  if (b_) b_->WorldTransform.on_change().disconnect(b_callback_);

  a_ = a;
  b_ = b;

  check();

  a_callback_ = a_->WorldTransform.on_change().connect(
        std::bind(&swift::ShapeTrigger::callback, this, std::placeholders::_1));
  b_callback_ = b_->WorldTransform.on_change().connect(
        std::bind(&swift::ShapeTrigger::callback, this, std::placeholders::_1));
}

////////////////////////////////////////////////////////////////////////////////

bool ShapeTrigger::callback(math::mat3 const& v) {
  check();
  return true;
}

////////////////////////////////////////////////////////////////////////////////

void ShapeTrigger::check() {

  bool intersects(a_->intersects(b_));

  if (intersects != intersects_) {
    intersects_ = intersects;

    if (intersects) on_enter.emit();
    else            on_leave.emit();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
