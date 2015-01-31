////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Component::Component()
  : Enabled(true)
  , user_(nullptr)
  , remove_flag_(false)
  , initialized_(false) {}

////////////////////////////////////////////////////////////////////////////////

Component::Component(Component const& to_copy)
  : Enabled(to_copy.Enabled())
  , user_(nullptr)
  , remove_flag_(to_copy.remove_flag_)
  , initialized_(to_copy.initialized_) {}

////////////////////////////////////////////////////////////////////////////////

void Component::detach(bool force) {
  if (user_) {
    user_->remove(this, force);
  }
}

////////////////////////////////////////////////////////////////////////////////

SceneObject const* Component::get_root() const {
  if (user_) {
    return user_->get_root();
  } else {
    return nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

SceneObject* Component::get_root() {
  if (user_) {
    return user_->get_root();
  } else {
    return nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Component::accept(SavableObjectVisitor& visitor) {
  visitor.add_member("Enabled", Enabled);
  visitor.add_member("Label", Label);
}

////////////////////////////////////////////////////////////////////////////////

}

