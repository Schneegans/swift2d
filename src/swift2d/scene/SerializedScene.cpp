////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
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
