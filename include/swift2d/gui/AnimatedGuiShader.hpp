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

#ifndef SWIFT2D_ANIMATED_GUI_SHADER_HPP
#define SWIFT2D_ANIMATED_GUI_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class AnimatedGuiShader : public Shader, public Singleton<AnimatedGuiShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<float>>       time;
  oglplus::Lazy<oglplus::Uniform<float>>       opacity;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>  size;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>  offset;
  oglplus::Lazy<oglplus::Uniform<int>>         diffuse;

  friend class Singleton<AnimatedGuiShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  AnimatedGuiShader();
  ~AnimatedGuiShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_ANIMATED_GUI_SHADER_HPP
