////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/DirectionalLightComponent.hpp>

#include <swift2d/materials/DirectionalLightShader.hpp>
#include <swift2d/geometries/Quad.hpp>


namespace swift {

////////////////////////////////////////////////////////////////////////////////

DirectionalLightComponent::DirectionalLightComponent()
  : Depth(0.f)
  , Direction(math::vec3(0, 0, -1))
  , Color(swift::Color(1.f, 1.f, 1.f)) {}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::draw(RenderContext const& ctx) {
  DirectionalLightShader::get().use(ctx);
  DirectionalLightShader::get().g_buffer_diffuse.Set(1);
  DirectionalLightShader::get().g_buffer_normal.Set(2);
  DirectionalLightShader::get().g_buffer_light.Set(3);
  DirectionalLightShader::get().light_dir.Set(math::normalized(Direction()));
  DirectionalLightShader::get().light_color.Set(Color().vec4());

  Quad::get().draw(ctx);
}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::serialize(SerializedScenePtr& scene) const {
  scene->lights.insert(std::make_pair(Depth.get(), create_copy()));
}

////////////////////////////////////////////////////////////////////////////////

void DirectionalLightComponent::accept(SavableObjectVisitor& visitor) {
  DrawableComponent::accept(visitor);
  visitor.add_member("Depth", Depth);
  visitor.add_member("Direction", Direction);
  visitor.add_member("Color", Color);
}

////////////////////////////////////////////////////////////////////////////////

}
