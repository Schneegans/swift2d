////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RendererPool.hpp>

#include <limits>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

RendererPool::RendererPool()
  : object_renderers_({
      &sprite_renderer,
      &fullscreen_sprite_renderer,
      &animated_sprite_renderer,
      &sprite_particle_system_renderer,
      &point_particle_system_renderer,
      &trail_system_renderer
    })
  , heat_renderers_({
      &heat_particle_system_renderer
    })
  , light_renderers_({}) {}

////////////////////////////////////////////////////////////////////////////////

void RendererPool::process_objects(RenderContext const& ctx) {
  process(ctx, object_renderers_);
}

////////////////////////////////////////////////////////////////////////////////

void RendererPool::process_heat(RenderContext const& ctx) {
  process(ctx, heat_renderers_);
}

////////////////////////////////////////////////////////////////////////////////

void RendererPool::process_light(RenderContext const& ctx) {
  process(ctx, light_renderers_);
}

////////////////////////////////////////////////////////////////////////////////

void RendererPool::process(RenderContext const& ctx, std::vector<ResourceRendererBase*> const& renderers) {
  float current_depth(std::numeric_limits<float>::max());
  bool any_object(false);

  for (auto& renderer: renderers) {
    any_object |= renderer->predraw(ctx, current_depth);
  }

  while(any_object) {
    float next_depth(current_depth);
    any_object = false;

    for (auto& renderer: renderers) {
      any_object |= renderer->draw(ctx, current_depth, next_depth);
    }

    current_depth = next_depth;
  }
}

////////////////////////////////////////////////////////////////////////////////

}
