////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_TRAIL_SYSTEM_COMPONENT_HPP
#define SWIFT2D_TRAIL_SYSTEM_COMPONENT_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/Component.hpp>
#include <swift2d/components/DepthComponent.hpp>
#include <swift2d/trails/TrailEmitterComponent.hpp>
#include <swift2d/graphics/ResourceRenderer.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TrailSystem;
typedef std::shared_ptr<TrailSystem>       TrailSystemPtr;

class TrailSystemComponent;
typedef std::shared_ptr<TrailSystemComponent>       TrailSystemComponentPtr;
typedef std::shared_ptr<const TrailSystemComponent> ConstTrailSystemComponentPtr;
typedef Property<TrailSystemComponentPtr>           TrailSystemComponentProperty;

// -----------------------------------------------------------------------------
class SWIFT_DLL TrailSystemComponent : public Component,
                                       public DepthComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------- inner classes
  struct Serialized : public SerializedComponent {
    float           StartWidth, EndWidth;
    float           StartGlow,  EndGlow;
    math::vec4      StartColor, EndColor;
    TexturePtr      Texture;
    float           TextureRepeat;
    bool            UseGlobalTexCoords;
    bool            BlendAdd;

    TrailSystemPtr System;
  };

  class Renderer : public ResourceRenderer<TrailSystemComponent> {
    void predraw(RenderContext const& ctx);
    void draw(RenderContext const& ctx, int start, int end);
  };

  // ---------------------------------------------------------------- properties
  Int32           MaxCount;

  Float           StartWidth, EndWidth;
  Float           StartGlow,  EndGlow;
  ColorProperty   StartColor, EndColor;
  TextureProperty Texture;
  Float           TextureRepeat;
  Bool            UseGlobalTexCoords;
  Bool            BlendAdd;

  // ----------------------------------------------------- contruction interface
  TrailSystemComponent();

  template <typename... Args>
  static TrailSystemComponentPtr create(Args&& ... a) {
    return std::make_shared<TrailSystemComponent>(a...);
  }

  // creates a copy from this
  TrailSystemComponentPtr create_copy() const {
    return std::make_shared<TrailSystemComponent>(*this);
  }

  ComponentPtr create_base_copy() const {
    return create_copy();
  }

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "TrailSystemComponent"; }

  void spawn(TrailSegment const& emitter);

  virtual void update(double time);
  virtual void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);

  friend class TrailEmitterComponent;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void add_emitter(TrailEmitterComponent* emitter);
  void remove_emitter(TrailEmitterComponent* emitter);

  TrailSystemPtr trail_system_;

  mutable std::vector<TrailSegment> new_segments_;
  mutable std::unordered_set<TrailEmitterComponent*> emitters_;
};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_TRAIL_SYSTEM_COMPONENT_HPP
