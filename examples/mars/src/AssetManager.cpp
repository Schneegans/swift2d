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

  TextureDatabase::instance()->add("light", Texture::create(Application::instance()->get_resource("images", "light.png")));

  auto mat = Material::create();
  mat->AnimatedDiffuseTexture = AnimatedTexture::create(Application::instance()->get_resource("images", "bomb.png"), 5, 4);
  mat->Emit = 1;
  mat->Glow = 0.5;
  MaterialDatabase::instance()->add("bullet", mat);
}

////////////////////////////////////////////////////////////////////////////////



