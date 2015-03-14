////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/components/TimedDeleteComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

TimedDeleteComponent::TimedDeleteComponent()
  : Life(1.0f) {}

////////////////////////////////////////////////////////////////////////////////

void TimedDeleteComponent::update(double time) {
  Life.set(Life.get()-time);
  if (Life.get() <= 0) {
    on_delete.emit();
    get_user()->detach();
  }
}

////////////////////////////////////////////////////////////////////////////////

void TimedDeleteComponent::accept(SavableObjectVisitor& visitor) {
  Component::accept(visitor);
  visitor.add_member("Life", Life);
}

////////////////////////////////////////////////////////////////////////////////

}
