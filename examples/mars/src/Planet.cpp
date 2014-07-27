////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/Planet.hpp"

////////////////////////////////////////////////////////////////////////////////

Planet::Planet()
  : Radius(0.5f)
  , Density(0.5)
  , Shinyness(0.2)
  , Emit(1.0)
  , Color(swift::Color(1, 1, 1, 1))
  , initialized_(false) {}

////////////////////////////////////////////////////////////////////////////////

void Planet::update(double time) {

  SceneObject::update(time);

  if (!initialized_) {
    initialized_ = true;

    auto get_texture = [&](std::string const& name) -> TexturePtr {

      if (name == "") {
        return TexturePtr();
      }

      if (TextureDatabase::instance()->has(name)) {
        return TextureDatabase::instance()->get(name);
      }

      auto tex = Texture::create(Application::instance()->get_resource("images", name));
      TextureDatabase::instance()->add(name, tex);

      return tex;
    };

    auto mat = SpriteMaterial::create();
    mat->DiffuseTexture = get_texture(DiffuseTex());
    mat->NormalTexture = get_texture(NormalTex());
    mat->EmitTexture = get_texture(EmitTex());
    mat->Shinyness = Shinyness();
    mat->Emit = Emit();

    auto tex = add<SpriteComponent>();
    tex->Depth = 0.0f;
    tex->Material = mat;

    auto dirt = add<PointParticleSystemComponent>();
    dirt->Depth = -0.01f;
    dirt->Scale = 3;
    dirt->LinearDamping = 0.05;
    dirt->StartColor = Color();
    dirt->EndColor = swift::Color(Color().r(), Color().g(), Color().b(), 0);
    dirt->Mass = 1;

    auto g_source = add<GravitySourceComponent>();
    g_source->Density = Density();

    auto body = add<StaticBodyComponent>();
    body->Radius = Radius();
    body->start_contact_with_dynamic.connect([this, dirt](DynamicBodyComponent* other, math::vec2 const& pos) {

      auto dir(pos - math::get_translation(WorldTransform()));
      float rot(std::atan2(dir.y(), dir.x()));
      float speed(other->get_linear_velocity().Length());

      if (speed > 0.5) {
        dirt->spawn_once({
          1.f, 0.5f,                    // life
          rot, 1.f,                     // direction
          speed, 1.5f,                    // velocity
          0.05f,                        // position
          100.f,                        // count
          math::make_translation(pos),  // world transform
          nullptr                       // one time emitter
        });
      }
    });
  }
}

////////////////////////////////////////////////////////////////////////////////

void Planet::accept(SavableObjectVisitor& visitor) {
  SceneObject::accept(visitor);
  visitor.add_member("DiffuseTex", DiffuseTex);
  visitor.add_member("NormalTex", NormalTex);
  visitor.add_member("EmitTex", EmitTex);
  visitor.add_member("Radius", Radius);
  visitor.add_member("Density", Density);
  visitor.add_member("Shinyness", Shinyness);
  visitor.add_member("Emit", Emit);
  visitor.add_member("Color", Color);
}

////////////////////////////////////////////////////////////////////////////////
