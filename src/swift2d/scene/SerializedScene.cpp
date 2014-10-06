////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// class header
#include <swift2d/scene/SerializedScene.hpp>

#include <swift2d/graphics/RendererPool.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SerializedScene::SerializedScene()
  : renderers_(new RendererPool()) {}

////////////////////////////////////////////////////////////////////////////////

SerializedScene::~SerializedScene() {
  delete renderers_;
}

////////////////////////////////////////////////////////////////////////////////

}
