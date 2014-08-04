////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/Explosion.hpp"
#include "../include/Litter.hpp"

bool Explosion::assets_loaded_ = false;

////////////////////////////////////////////////////////////////////////////////

Explosion::Explosion(math::vec2 const& pos)
  : initialized_(false) {

  Transform = math::make_translation(pos);
}

////////////////////////////////////////////////////////////////////////////////

void Explosion::update(double time) {

  if (!assets_loaded_) {
    assets_loaded_ = true;

    auto get_texture = [&](std::string const& name) -> AnimatedTexturePtr {

      if (TextureDatabase::instance()->has(name)) {
        return std::dynamic_pointer_cast<AnimatedTexture>(TextureDatabase::instance()->get(name));
      }

      auto tex = AnimatedTexture::create(Application::instance()->get_resource("images", name), 8, 4);
      TextureDatabase::instance()->add(name, tex);

      return tex;
    };

    auto mat = Material::create();
    mat->AnimatedDiffuseTexture.set(get_texture("explosion.png"));
    mat->Emit = 1.0;
    mat->Glow = 1.0;
    MaterialDatabase::instance()->add("explosion", mat);
  }

  if (!initialized_) {
    initialized_ = true;

    auto deleter = add<TimedDeleteBehavior>();
         deleter->Life = 0.65f;

    auto flash = add<PointLightComponent>();
         flash->Texture = TextureDatabase::instance()->get("light");
         flash->Transform = math::make_scale(3);
         flash->Color = Color(1, 1, 0.6, 1);

    auto tex = add<AnimatedSpriteComponent>();
         tex->Transform = math::make_scale(1);
         tex->Depth = 0.1f;
         tex->Material.set(MaterialDatabase::instance()->get("explosion"));
         tex->Time.set(1, 0.7);
         tex->Time.set_direction(AnimatedFloat::LINEAR);
         tex->Time.on_change().connect([flash](float val){
           auto c = flash->Color();
           flash->Color = Color(c.r(), c.g(), c.b(), 0.7-val);
         });

    for (int i(0); i<10; ++i) {
      float r(math::random::get(0.0f, 2*M_PI));
      float s(0.05);
      math::vec2 dir(sin(r)*s, cos(r)*s);
      Parent()->add_object(std::make_shared<Litter>(math::get_translation(Transform()), dir));
    }
  }

  SceneObject::update(time);
}

////////////////////////////////////////////////////////////////////////////////

void Explosion::accept(SavableObjectVisitor& visitor) {
  SceneObject::accept(visitor);
}

////////////////////////////////////////////////////////////////////////////////
