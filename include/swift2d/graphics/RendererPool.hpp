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
#include <swift2d/graphics/AnimatedSpriteRenderer.hpp>

#include <swift2d/particles/SpriteParticleSystemRenderer.hpp>
#include <swift2d/particles/PointParticleSystemRenderer.hpp>
#include <swift2d/particles/HeatParticleSystemRenderer.hpp>

#include <swift2d/trails/TrailSystemRenderer.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class SWIFT_DLL RendererPool {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ---------------------------------------------------------- object renderers
  SpriteComponent::Renderer    sprite_renderer;
  FullscreenSpriteComponent::Renderer    fullscreen_sprite_renderer;
  AnimatedSpriteRenderer       animated_sprite_renderer;

  SpriteParticleSystemRenderer sprite_particle_system_renderer;
  PointParticleSystemRenderer  point_particle_system_renderer;

  TrailSystemRenderer          trail_system_renderer;

  // ------------------------------------------------------------ heat renderers
  HeatParticleSystemRenderer   heat_particle_system_renderer;

  // ----------------------------------------------------------- light renderers

  // ---------------------------------------------------- construction interface
  RendererPool();

  // ------------------------------------------------------------ public methods
  void process_objects(RenderContext const& ctx);
  void process_heat(RenderContext const& ctx);
  void process_light(RenderContext const& ctx);

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void process(RenderContext const& ctx, std::vector<ResourceRendererBase*> const& renderers);

  std::vector<ResourceRendererBase*> object_renderers_;
  std::vector<ResourceRendererBase*> heat_renderers_;
  std::vector<ResourceRendererBase*> light_renderers_;
};

}

#endif  // SWIFT2D_RENDERER_POOL_HPP
