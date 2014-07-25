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

math::vec2 CameraComponent::gui_to_world(math::vec2 const& gui_pos) const {
  auto w = WindowManager::instance()->get_default();
  if (w->get_context().ready) {
    math::vec2 scaled = (gui_pos / math::vec2(w->get_context().size));
    math::vec3 tmp = WorldTransform() * math::vec3(scaled.x(), scaled.y(), 1.f);
    return math::vec2(tmp.x(), tmp.y());
  }
  return math::vec2(0.f, 0.f);
}

////////////////////////////////////////////////////////////////////////////////

math::vec2 CameraComponent::world_to_gui(math::vec2 const& world_pos) const {
  auto w = WindowManager::instance()->get_default();
  math::vec3 tmp = math::inversed(WorldTransform()) * math::vec3(world_pos.x(), world_pos.y(), 1.f);
  math::vec2 unscaled(tmp.x(), tmp.y());
  return unscaled * math::vec2(w->get_context().size) / math::vec2(Size()) + math::vec2(w->get_context().size)*0.5;
}

////////////////////////////////////////////////////////////////////////////////

void CameraComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  visitor.add_member("Size", Size);
  visitor.add_member("Parallax", Parallax);
}

////////////////////////////////////////////////////////////////////////////////

}
