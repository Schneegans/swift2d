////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "SpaceScene.hpp"

using namespace swift;

////////////////////////////////////////////////////////////////////////////////

swift::SceneObjectPtr SpaceScene::create() {

  TextureDatabase::get().add("smoke", Texture::create(Paths::get().resource("images", "smoke.png")));
  TextureDatabase::get().add("fire",  Texture::create(Paths::get().resource("images", "fire.png")));
  TextureDatabase::get().add("light",  Texture::create(Paths::get().resource("images", "light.png")));

  auto mat = Material::create();
  mat->DiffuseTexture = Texture::create(Paths::get().resource("images", "bg.jpg"));
  MaterialDatabase::get().add("background", mat);

  mat = Material::create();
  mat->DiffuseTexture = Texture::create(Paths::get().resource("images", "ship.jpg"));
  MaterialDatabase::get().add("ship", mat);

  mat = Material::create();
  mat->DiffuseTexture = Texture::create(Paths::get().resource("images", "bullet.jpg"));
  MaterialDatabase::get().add("bullet", mat);

  mat = Material::create();
  mat->DiffuseTexture = Texture::create(Paths::get().resource("images", "planet_diffuse2.png"));
  mat->NormalTexture = Texture::create(Paths::get().resource("images", "planet_normal2.png"));
  MaterialDatabase::get().add("planet1", mat);

  mat = Material::create();
  mat->DiffuseTexture = Texture::create(Paths::get().resource("images", "planet_diffuse.png"));
  mat->NormalTexture = Texture::create(Paths::get().resource("images", "planet_normal.png"));
  MaterialDatabase::get().add("planet2", mat);

  // example scene setup -------------------------------------------------------
  auto scene = SceneManager::get().current()->Root;

  auto field = scene->add<CircularShape>();
       field->Transform = math::make_scale(4);

  auto sun = scene->add<DirectionalLightComponent>();
       sun->Transform = math::make_scale(15) * math::make_translation(-0.2, 0.2);
       sun->Direction = math::vec3(1.f, 1.f, -1.f);

  auto bg = scene->add<SpriteComponent>();
       bg->Depth = -1000.0f;
       bg->Material = MaterialDatabase::get().lookup("background");
       bg->Transform = math::make_scale(2.f);

  // planet
  auto planet1 = scene->add_object();
       planet1->Transform = math::make_translation(-0.9, -0.5);
  auto sprite2 = planet1->add<SpriteComponent>();
       sprite2->Depth = 0.0f;
       sprite2->Material = MaterialDatabase::get().lookup("planet1");

  auto planet2 = scene->add_object();
       planet2->Transform = math::make_translation(-1.2, 1.0);
  auto sprite3 = planet2->add<SpriteComponent>();
       sprite3->Depth = 0.0f;
       sprite3->Material = MaterialDatabase::get().lookup("planet2");

  return scene;
}

////////////////////////////////////////////////////////////////////////////////

