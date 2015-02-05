////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TANGIBLE_POINTER_COMPONENT_HPP
#define SWIFT2D_TANGIBLE_POINTER_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/components/DepthComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TangiblePointerComponent;
typedef std::shared_ptr<TangiblePointerComponent>       TangiblePointerComponentPtr;
typedef std::shared_ptr<const TangiblePointerComponent> ConstTangiblePointerComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL TangiblePointerComponent : public Component,
                                           public DepthComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public SerializedComponent {
    TexturePtr Texture;
    math::vec2 Size;
    math::vec2 Position;
    float      MinDistance;
    float      MaxDistance;
  };

  class Renderer : public ResourceRenderer<TangiblePointerComponent> {
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  TexturePtr Texture;
  Vec2i      Size;
  Float      MinDistance;
  Float      MaxDistance;


  // ---------------------------------------------------- construction interface
  TangiblePointerComponent();

  static TangiblePointerComponentPtr create() {
    return std::make_shared<TangiblePointerComponent>();
  }

  // creates a copy from this
  TangiblePointerComponentPtr create_copy() const {
    return std::make_shared<TangiblePointerComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual void update(double time);

  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "TangiblePointerComponent"; }

  void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_TANGIBLE_POINTER_COMPONENT_HPP
