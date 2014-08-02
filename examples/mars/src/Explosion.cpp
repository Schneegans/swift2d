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
    mat->Glow = 0.0;
    MaterialDatabase::instance()->add("explosion", mat);
  }

  if (!initialized_) {
    initialized_ = true;

    auto deleter = add<TimedDeleteBehavior>();
         deleter->Life = 1.9f;

    auto tex = add<AnimatedSpriteComponent>();
         tex->Transform = math::make_scale(2);
         tex->Depth = 0.1f;
         tex->Material.set(MaterialDatabase::instance()->get("explosion"));
         tex->Time.set(1, 2);
         tex->Time.set_direction(AnimatedFloat::LINEAR);

    for (int i(0); i<20; ++i) {
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
