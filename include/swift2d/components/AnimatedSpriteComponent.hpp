////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_ANIMATED_SPRITE_COMPONENT_HPP
#define SWIFT2D_ANIMATED_SPRITE_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/SpriteComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class AnimatedSpriteComponent;
typedef std::shared_ptr<AnimatedSpriteComponent>       AnimatedSpriteComponentPtr;
typedef std::shared_ptr<const AnimatedSpriteComponent> ConstAnimatedSpriteComponentPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL AnimatedSpriteComponent : public SpriteComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  AnimatedFloat Time;

  // ----------------------------------------------------- contruction interface
  AnimatedSpriteComponent();

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static AnimatedSpriteComponentPtr create(Args&& ... a) {
    return std::make_shared<AnimatedSpriteComponent>(a...);
  }

  // creates a copy from this
  AnimatedSpriteComponentPtr create_copy() const {
    return std::make_shared<AnimatedSpriteComponent>(*this);
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "AnimatedSpriteComponent"; }

  virtual void update(double time);

  virtual void draw(RenderContext const& ctx);
  virtual void draw_instanced(RenderContext const& ctx, std::vector<math::mat3> const& transforms);

  virtual void serialize(SerializedScenePtr& scene) const;

  virtual void accept(SavableObjectVisitor& visitor);
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_ANIMATED_SPRITE_COMPONENT_HPP
