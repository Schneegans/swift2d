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
#include <swift2d/components/DrawableComponent.hpp>
#include <swift2d/trails/TrailSystem.hpp>
#include <swift2d/textures/Texture.hpp>
#include <swift2d/utils/Color.hpp>

#include <unordered_set>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class TrailSystemComponent;
typedef std::shared_ptr<TrailSystemComponent>       TrailSystemComponentPtr;
typedef std::shared_ptr<const TrailSystemComponent> ConstTrailSystemComponentPtr;

// -----------------------------------------------------------------------------
class TrailSystemComponent : public DrawableComponent {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------------- properties
  Int             MaxCount;
  Float           Depth;

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

  // ------------------------------------------------------------ public methods
  virtual std::string get_type_name() const {  return get_type_name_static(); }
  static  std::string get_type_name_static() { return "TrailSystemComponent"; }


  void add_emitter(TrailEmitterComponent const* emitter);
  void remove_emitter(TrailEmitterComponent const* emitter);

  void draw(RenderContext const& ctx);

  virtual void serialize(SerializedScenePtr& scene) const;
  virtual void accept(SavableObjectVisitor& visitor);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  TrailSystemPtr trail_system_;

  std::unordered_set<TrailEmitterComponent const*> emitters_;
  mutable std::vector<SerializedTrailEmitter>      serialized_emitters_;

};

// -----------------------------------------------------------------------------

}

#endif  // SWIFT2D_TRAIL_SYSTEM_COMPONENT_HPP
