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

  TextureDatabase::instance()->add("bullet", Texture::create(Application::instance()->get_resource("images", "asteroid_diffuse0.png")));
  TextureDatabase::instance()->add("bulletn", Texture::create(Application::instance()->get_resource("images", "asteroid_normal0.png")));

  auto mat = Material::create();
  mat->DiffuseTexture = TextureDatabase::instance()->get("bullet");
  mat->NormalTexture = TextureDatabase::instance()->get("bulletn");
  mat->Emit = 0.3;
  mat->Glow = 0.0;
  MaterialDatabase::instance()->add("bullet", mat);
}

////////////////////////////////////////////////////////////////////////////////



