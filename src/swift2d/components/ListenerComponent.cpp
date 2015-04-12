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

#include <swift2d/components/ListenerComponent.hpp>

#include <swift2d/scene/SceneObject.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ListenerComponent::ListenerComponent() {

  Volume.on_change().connect([&](float val){
    listener_.Gain(val);
    return true;
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
