////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_HEAT_SPRITE_SHADER_HPP
#define SWIFT2D_HEAT_SPRITE_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class HeatSpriteShader : public Shader,
                         public Singleton<HeatSpriteShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   projection;
  oglplus::Lazy<oglplus::Uniform<float>>        parallax;
  oglplus::Lazy<oglplus::Uniform<float>>        depth;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   transform;
  oglplus::Lazy<oglplus::Uniform<math::mat3>>   heat_transform;
  oglplus::Lazy<oglplus::Uniform<int>>          diffuse;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>   texcoord_offset_scale;
  oglplus::Lazy<oglplus::Uniform<float>>        opacity;

  friend class Singleton<HeatSpriteShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  HeatSpriteShader();
  ~HeatSpriteShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_HEAT_SPRITE_SHADER_HPP
