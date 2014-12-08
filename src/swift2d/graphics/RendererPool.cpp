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
  : objects_({
      &sprites,
      &fullscreen_sprites,
      &animated_sprites,
      &sprite_particle_systems,
      &point_particle_systems,
      &trail_systems
    })
  , heats_({
      &heat_particle_systems,
      &heat_sprites
    })
  , lights_({
      &directional_lights,
      &point_lights,
      &light_particle_systems
    })
  , guis_({
      &gui_elements,
      &gui_sprite_elements,
      &animated_gui_sprite_elements,
      &tangible_pointers
    }) {}

////////////////////////////////////////////////////////////////////////////////

void RendererPool::process_objects(RenderContext const& ctx) {
  process(ctx, objects_);
}

////////////////////////////////////////////////////////////////////////////////

void RendererPool::process_heat(RenderContext const& ctx) {
  process(ctx, heats_);
}

////////////////////////////////////////////////////////////////////////////////

void RendererPool::process_light(RenderContext const& ctx) {
  process(ctx, lights_);
}

////////////////////////////////////////////////////////////////////////////////

void RendererPool::process_gui(RenderContext const& ctx) {
  process(ctx, guis_);
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
