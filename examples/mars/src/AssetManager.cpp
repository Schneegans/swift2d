////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include "../include/AssetManager.hpp"

////////////////////////////////////////////////////////////////////////////////

AssetManager::AssetManager() {

  TextureDatabase::instance()->add("point_light", Texture::create(Application::instance()->get_resource("images", "light.png")));
  TextureDatabase::instance()->add("bullet", Texture::create(Application::instance()->get_resource("images", "asteroid_diffuse0.png")));
  TextureDatabase::instance()->add("bulletn", Texture::create(Application::instance()->get_resource("images", "asteroid_normal0.png")));

  auto mat = SpriteMaterial::create();
  mat->DiffuseTexture = TextureDatabase::instance()->get("bullet");
  mat->NormalTexture = TextureDatabase::instance()->get("bulletn");
  mat->Emit = 0.3;
  mat->Glow = 0.0;
  SpriteMaterialDatabase::instance()->add("bullet", mat);

  auto mat2 = PointLightMaterial::create_from_database("point_light");
  mat2->Color = Color(0.4, 0.3, 1.0);
  LightMaterialDatabase::instance()->add("light", mat2);

  LightMaterialDatabase::instance()->add("sun1",  DirectionalLightMaterial::create(math::vec3(0.1, 1, 0), Color(1, 0.5, 1.0)));
  LightMaterialDatabase::instance()->add("sun2",  DirectionalLightMaterial::create(math::vec3(-1, -1, 0), Color(0.4, 0.8, 1.0)));
}

////////////////////////////////////////////////////////////////////////////////



