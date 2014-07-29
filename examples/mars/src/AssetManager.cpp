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

  auto mat1 = SpriteMaterial::create();
  mat1->DiffuseTexture = TextureDatabase::instance()->get("bullet");
  mat1->NormalTexture = TextureDatabase::instance()->get("bulletn");
  mat1->Emit = 0.3;
  mat1->Glow = 0.0;
  MaterialDatabase::instance()->add("bullet", mat1);

  auto mat2 = PointLightMaterial::create_from_database("point_light");
  mat2->Color = Color(0.4, 0.3, 1.0);
  MaterialDatabase::instance()->add("light", mat2);

  MaterialDatabase::instance()->add("sun1",  DirectionalLightMaterial::create(math::vec3(0.1, 1, 0), Color(1, 0.5, 1.0)));
  MaterialDatabase::instance()->add("sun2",  DirectionalLightMaterial::create(math::vec3(-1, -1, 0), Color(0.4, 0.8, 1.0)));
}

////////////////////////////////////////////////////////////////////////////////



