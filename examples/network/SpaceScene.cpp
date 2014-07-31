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

  TextureDatabase::instance()->add("smoke", Texture::create(Application::instance()->get_resource("images", "smoke.png")));
  TextureDatabase::instance()->add("fire",  Texture::create(Application::instance()->get_resource("images", "fire.png")));

  auto mat = SpriteMaterial::create();
  mat->DiffuseTexture = Texture::create(Application::instance()->get_resource("images", "bg.jpg"));
  SpriteMaterialDatabase::instance()->add("background", mat);

  mat = SpriteMaterial::create();
  mat->DiffuseTexture = Texture::create(Application::instance()->get_resource("images", "ship.jpg"));
  SpriteMaterialDatabase::instance()->add("ship", mat);

  mat = SpriteMaterial::create();
  mat->DiffuseTexture = Texture::create(Application::instance()->get_resource("images", "bullet.jpg"));
  SpriteMaterialDatabase::instance()->add("bullet", mat);

  mat = SpriteMaterial::create();
  mat->DiffuseTexture = Texture::create(Application::instance()->get_resource("images", "planet_diffuse2.png"));
  mat->NormalTexture = Texture::create(Application::instance()->get_resource("images", "planet_normal2.png"));
  SpriteMaterialDatabase::instance()->add("planet1", mat);

  mat = SpriteMaterial::create();
  mat->DiffuseTexture = Texture::create(Application::instance()->get_resource("images", "planet_diffuse.png"));
  mat->NormalTexture = Texture::create(Application::instance()->get_resource("images", "planet_normal.png"));
  SpriteMaterialDatabase::instance()->add("planet2", mat);

  LightMaterialDatabase::instance()->add("light",      PointLightMaterial::create_from_file(Application::instance()->get_resource("images", "light.png")));
  LightMaterialDatabase::instance()->add("sun",        DirectionalLightMaterial::create(math::vec3(1, 1, -1)));

  // example scene setup -------------------------------------------------------
  auto scene = SceneManager::instance()->get_default();

  // auto music = scene->add<SoundComponent>();
  //      music->Sound = Sound::create_from_file(Application::instance()->get_resource("audio", "music.ogg"));
  //      music->Volume = 0.1f;
  //      music->play();

  auto field = scene->add<CircularShape>();
       field->Transform = math::make_scale(4);

  auto sun = scene->add<LightComponent>();
       sun->Transform = math::make_scale(15) * math::make_translation(-0.2, 0.2);
       sun->Material = LightMaterialDatabase::instance()->get("sun");

  auto bg = scene->add<SpriteComponent>();
       bg->Depth = -1000.0f;
       bg->Material = SpriteMaterialDatabase::instance()->get("background");
       bg->Transform = math::make_scale(2.f);

  // planet
  auto planet1 = scene->add_object();
       planet1->Transform = math::make_translation(-0.9, -0.5);
  auto sprite2 = planet1->add<SpriteComponent>();
       sprite2->Depth = 0.0f;
       sprite2->Material = SpriteMaterialDatabase::instance()->get("planet1");

  auto planet2 = scene->add_object();
       planet2->Transform = math::make_translation(-1.2, 1.0);
  auto sprite3 = planet2->add<SpriteComponent>();
       sprite3->Depth = 0.0f;
       sprite3->Material = SpriteMaterialDatabase::instance()->get("planet2");

  return scene;
}

////////////////////////////////////////////////////////////////////////////////

