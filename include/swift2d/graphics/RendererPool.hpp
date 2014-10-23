////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_RENDERER_POOL_HPP
#define SWIFT2D_RENDERER_POOL_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/components/SpriteComponent.hpp>
#include <swift2d/components/FullscreenSpriteComponent.hpp>
#include <swift2d/components/AnimatedSpriteComponent.hpp>
#include <swift2d/components/DirectionalLightComponent.hpp>
#include <swift2d/components/PointLightComponent.hpp>

#include <swift2d/particles/SpriteParticleSystemComponent.hpp>
#include <swift2d/particles/PointParticleSystemComponent.hpp>
#include <swift2d/particles/HeatParticleSystemComponent.hpp>
#include <swift2d/particles/LightParticleSystemComponent.hpp>

#include <swift2d/gui/GuiComponent.hpp>

#include <swift2d/trails/TrailSystemComponent.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL RendererPool {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------- object renderers
  SpriteComponent::               Renderer sprites;
  FullscreenSpriteComponent::     Renderer fullscreen_sprites;
  AnimatedSpriteComponent::       Renderer animated_sprites;

  SpriteParticleSystemComponent:: Renderer sprite_particle_systems;
  PointParticleSystemComponent::  Renderer point_particle_systems;

  TrailSystemComponent::          Renderer trail_systems;

  // ------------------------------------------------------------ heat renderers
  HeatParticleSystemComponent::   Renderer heat_particle_systems;

  // ----------------------------------------------------------- light renderers
  DirectionalLightComponent::     Renderer directional_lights;
  PointLightComponent::           Renderer point_lights;
  LightParticleSystemComponent::  Renderer light_particle_systems;

  // ------------------------------------------------------------- gui renderers
  GuiComponent::                  Renderer gui_elements;

  // ---------------------------------------------------- construction interface
  RendererPool();

  // ------------------------------------------------------------ public methods
  void process_objects(RenderContext const& ctx);
  void process_heat(RenderContext const& ctx);
  void process_light(RenderContext const& ctx);
  void process_gui(RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void process(RenderContext const& ctx, std::vector<ResourceRendererBase*> const& renderers);

  std::vector<ResourceRendererBase*> objects_;
  std::vector<ResourceRendererBase*> heats_;
  std::vector<ResourceRendererBase*> lights_;
  std::vector<ResourceRendererBase*> guis_;
};

}

#endif  // SWIFT2D_RENDERER_POOL_HPP
