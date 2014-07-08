////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SPRITE_COMPONENT_HPP
#define SWIFT2D_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/materials/Material.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class SpriteComponent;
typedef std::shared_ptr<SpriteComponent>       SpriteComponentPtr;
typedef std::shared_ptr<const SpriteComponent> ConstSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SpriteComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Float            Depth;
  MaterialProperty Material;

  // ----------------------------------------------------- contruction interface
  SpriteComponent() : Depth(0.f), Material(nullptr) {}

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static SpriteComponentPtr create(Args&& ... a) {
    return std::make_shared<SpriteComponent>(a...);
  }

  // creates a copy from this
  SpriteComponentPtr create_copy() const {
    return std::make_shared<SpriteComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "SpriteComponent"; }

  void draw(RenderContext const& ctx) {
    Material()->draw_quad(ctx, WorldTransform(), Depth());
  }

  void serialize(SerializedScenePtr& scene) const {
    scene->objects.insert(std::make_pair(Depth.get(), create_copy()));
  }

  virtual void accept(SavableObjectVisitor& visitor) {
    DrawableComponent::accept(visitor);
    visitor.add_member("Depth", Depth);
    visitor.add_object("Material", Material);
  }

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_SPRITE_COMPONENT_HPP
