////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/components/ListenerComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ListenerComponent::ListenerComponent() {

  Volume.on_change().connect([&](float val){
    listener_.Gain(val);
  });

  Volume = 1.0f;
}

////////////////////////////////////////////////////////////////////////////////

void ListenerComponent::update(double time) {
  TransformableComponent::update(time);

  auto pos(get_world_position());
  listener_.Position(pos.x(), pos.y(), 0);
}

////////////////////////////////////////////////////////////////////////////////

void ListenerComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Volume", Volume);
}

////////////////////////////////////////////////////////////////////////////////

}
