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
  SpriteComponent();

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

  virtual void draw(RenderContext const& ctx);

  virtual void serialize(SerializedScenePtr& scene) const;

  virtual void accept(SavableObjectVisitor& visitor);

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_SPRITE_COMPONENT_HPP
