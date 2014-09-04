////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/CameraComponent.hpp>
#include <swift2d/graphics/WindowManager.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

CameraComponent::CameraComponent()
  : Size(math::vec2(1.f, 1.f))
  , Parallax(1.0) {}

////////////////////////////////////////////////////////////////////////////////

math::vec2 CameraComponent::pixel_to_world(math::vec2 const& pixel_pos) const {
  auto w = WindowManager::get().current();
  if (w->get_context().ready) {
    math::vec2 scaled = (pixel_pos / math::vec2(w->get_context().window_size));
    math::vec3 tmp = WorldTransform() * math::vec3(scaled.x(), scaled.y(), 1.f);
    return math::vec2(tmp.x(), tmp.y());
  }
  return math::vec2(0.f, 0.f);
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 CameraComponent::world_to_pixel(math::vec2 const& world_pos) const {
  auto w = WindowManager::get().current();
  math::vec3 tmp = math::inversed(WorldTransform()) * math::vec3(world_pos.x(), world_pos.y(), 1.f);
  return tmp.xy() * math::vec2(w->get_context().window_size) / (2.0f*Size()) + math::vec2(w->get_context().window_size)*0.5f;
}

////////////////////////////////////////////////////////////////////////////////

void CameraComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Size", Size);
  visitor.add_member("Parallax", Parallax);
}

////////////////////////////////////////////////////////////////////////////////

}
