////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/components/ListenerComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ListenerComponent::ListenerComponent() {

  pVolume.on_change().connect([&](float val){
    listener_.Gain(val);
  });

  pVolume = 1.0f;
}

////////////////////////////////////////////////////////////////////////////////

void ListenerComponent::update(double time) {
  TransformableComponent::update(time);

  auto pos(get_world_position());
  listener_.Position(pos.x(), pos.y(), 0);
}

////////////////////////////////////////////////////////////////////////////////

}
