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

swift::SceneObjectPtr SpaceScene::create(swift::Application const& app) {

  TextureDatabase::instance()->add("smoke", Texture::create(app.get_resource("images", "smoke.png")));
  TextureDatabase::instance()->add("fire", Texture::create(app.get_resource("images", "fire.png")));

  MaterialDatabase::instance()->add("background", ShadelessTextureMaterial::create_from_file(app.get_resource("images", "bg.jpg")));
  MaterialDatabase::instance()->add("ship",       ShadelessTextureMaterial::create_from_file(app.get_resource("images", "ship.png")));
  MaterialDatabase::instance()->add("bullet",     ShadelessTextureMaterial::create_from_file(app.get_resource("images", "bullet.png")));

  MaterialDatabase::instance()->add("planet1",    BumpTextureMaterial::create_from_files(app.get_resource("images", "planet_diffuse2.png"), app.get_resource("images", "planet_normal2.png")));
  MaterialDatabase::instance()->add("planet2",    BumpTextureMaterial::create_from_files(app.get_resource("images", "planet_diffuse.png"), app.get_resource("images", "planet_normal.png")));

  MaterialDatabase::instance()->add("light",      PointLightMaterial::create_from_file(app.get_resource("images", "light.png")));
  MaterialDatabase::instance()->add("sun",        DirectionalLightMaterial::create(math::vec3(1, 1, -1)));

  // example scene setup -------------------------------------------------------
  auto scene = SceneManager::instance()->get_default();

  // auto music = scene->add<SoundComponent>();
  //      music->Sound = Sound::create_from_file(app.get_resource("audio", "music.ogg"));
  //      music->Volume = 0.1f;
  //      music->play();

  auto field = scene->add<CircularShape>();
       field->Transform = math::make_scale(4);

  auto sun = scene->add<LightComponent>();
       sun->Transform = math::make_scale(15) * math::make_translate(-0.2, 0.2);
       sun->Material = MaterialDatabase::instance()->get("sun");

  auto bg = scene->add<SpriteComponent>();
       bg->Depth = -1000.0f;
       bg->Material = MaterialDatabase::instance()->get("background");
       bg->Transform = math::make_scale(2.f);

  // planet
  auto planet1 = scene->add_object();
       planet1->Transform = math::make_translate(-0.9, -0.5);
  auto sprite2 = planet1->add<SpriteComponent>();
       sprite2->Depth = 0.0f;
       sprite2->Material = MaterialDatabase::instance()->get("planet1");

  auto planet2 = scene->add_object();
       planet2->Transform = math::make_translate(-1.2, 1.0);
  auto sprite3 = planet2->add<SpriteComponent>();
       sprite3->Depth = 0.0f;
       sprite3->Material = MaterialDatabase::instance()->get("planet2");

  return scene;
}

////////////////////////////////////////////////////////////////////////////////

