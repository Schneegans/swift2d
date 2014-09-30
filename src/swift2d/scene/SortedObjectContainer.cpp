////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <swift2d/scene/SortedObjectContainer.hpp>

#include <swift2d/components/DrawableComponent.hpp>


namespace swift {

void SortedObjectContainer::add_object(DrawableComponentPtr const& object) {
  objects_.push_back(object);
}

void SortedObjectContainer::add_instanced_object(void* key, DrawableComponentPtr const& object) {
  instanced_objects_[key].first = object;
  instanced_objects_[key].second.push_back(object->WorldTransform());
}

std::map<void*, std::pair<DrawableComponentPtr, std::vector<math::mat3>>> const& SortedObjectContainer::get_instanced_objects() const {
  return instanced_objects_;
}

std::vector<DrawableComponentPtr> const& SortedObjectContainer::get_objects() const {
  return objects_;
}

}

