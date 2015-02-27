////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/behaviors/TimedDeleteBehavior.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TimedDeleteBehavior::TimedDeleteBehavior()
  : Life(1.0f) {}

////////////////////////////////////////////////////////////////////////////////

void TimedDeleteBehavior::update(double time) {
  Life.set(Life.get()-time);
  if (Life.get() <= 0) {
    on_delete.emit();
    get_user()->detach();
  }
}

////////////////////////////////////////////////////////////////////////////////

void TimedDeleteBehavior::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("Life", Life);
}

////////////////////////////////////////////////////////////////////////////////

}
