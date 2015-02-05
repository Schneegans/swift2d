////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/components/HeatSpriteComponent.hpp>

#include <swift2d/materials/HeatSpriteShader.hpp>
#include <swift2d/graphics/RendererPool.hpp>
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/databases/TextureDatabase.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

HeatSpriteComponent::HeatSpriteComponent()
  : Opacity(1.f)
  , TexcoordScale(math::vec2(1.f, 1.f)) {

  TextureName.on_change().connect([this](std::string const& val) {
    this->Texture = TextureDatabase::get().lookup_or_load(val);
    return true;
  });
}

////////////////////////////////////////////////////////////////////////////////

void HeatSpriteComponent::update(double time) {
  TransformableComponent::update(time);
  DepthComponent::update(time, get_user());
}

////////////////////////////////////////////////////////////////////////////////

void HeatSpriteComponent::serialize(SerializedScenePtr& scene) const {
  Serialized s;
  s.Depth         = WorldDepth();
  s.Transform     = WorldTransform();
  s.Texture       = Texture();
  s.TexcoordOffset = TexcoordOffset();
  s.TexcoordScale  = TexcoordScale();
  s.Opacity       = Opacity();
  scene->renderers().heat_sprites.add(std::move(s));
}

////////////////////////////////////////////////////////////////////////////////

void HeatSpriteComponent::accept(SavableObjectVisitor& visitor) {
  TransformableComponent::accept(visitor);
  DepthComponent::accept(visitor);
  visitor.add_member("TextureName", TextureName);
  visitor.add_object_property("Texture", Texture);
  visitor.add_member("TexcoordOffset", TexcoordOffset);
  visitor.add_member("TexcoordScale", TexcoordScale);
  visitor.add_member("Opacity", Opacity);
}

////////////////////////////////////////////////////////////////////////////////

void HeatSpriteComponent::Renderer::draw(RenderContext const& ctx, int start, int end) {
  std::sort(objects.begin() + start, objects.begin() + end,
    [](HeatSpriteComponent::Serialized const& a, HeatSpriteComponent::Serialized const& b){
      return a.Texture.get() < b.Texture.get();
    });

  while (start < end) {

    auto& object(objects[start]);
    auto& tex(object.Texture);

    SWIFT_PUSH_GL_RANGE("Draw HeatSprites");

    std::vector<math::mat3> transform;
    std::vector<math::vec4> texcoord_offset_scale;
    std::vector<math::mat3> heat_transform;
    std::vector<float>      opacity;

    while (start < end && objects[start].Texture == tex) {
      transform.push_back(objects[start].Transform);
      heat_transform.push_back(math::make_rotation(math::get_rotation(objects[start].Transform)));
      texcoord_offset_scale.push_back(math::vec4(
        objects[start].TexcoordOffset.x(),
        objects[start].TexcoordOffset.y(),
        objects[start].TexcoordScale.x(),
        objects[start].TexcoordScale.y()
      ));
      opacity.push_back(objects[start].Opacity);
      ++start;
    }

    auto& shader(HeatSpriteShader::get());

    object.Texture->bind(ctx, 0);

    shader.use(ctx);
    shader.diffuse.    Set(0);
    shader.projection. Set(ctx.projection_matrix);
    shader.depth.      Set(object.Depth);
    shader.parallax.   Set(ctx.projection_parallax);

    int index(0);

    while (index < transform.size()) {
      std::size_t count(std::min(100, (int)transform.size()-index));

      shader.transform.Set(count, (const math::mat3*)&transform[index]);
      shader.texcoord_offset_scale.Set(count, (const math::vec4*)&texcoord_offset_scale[index]);
      shader.heat_transform.Set(count, (const math::mat3*)&heat_transform[index]);
      shader.opacity.Set(count, (const float*)&opacity[index]);

      Quad::get().draw(ctx, count);

      index += count;
    }

    SWIFT_POP_GL_RANGE();
  }
}

////////////////////////////////////////////////////////////////////////////////

}
