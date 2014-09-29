////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_COLORED_TRAIL_SHADER_HPP
#define SWIFT2D_COLORED_TRAIL_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TrailShaderBase.hpp>
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Singleton.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class ColoredTrailShader : public TrailShaderBase,
                           public Shader,
                           public Singleton<ColoredTrailShader> {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ------------------------------------------------------------------ uniforms
  oglplus::Lazy<oglplus::Uniform<math::mat3>>  projection;
  oglplus::Lazy<oglplus::Uniform<float>>       start_width;
  oglplus::Lazy<oglplus::Uniform<float>>       end_width;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  start_color;
  oglplus::Lazy<oglplus::Uniform<math::vec4>>  end_color;
  oglplus::Lazy<oglplus::Uniform<math::vec2>>  glow;
  oglplus::Lazy<oglplus::Uniform<float>>       total_time;
  oglplus::Lazy<oglplus::Uniform<int>>         use_global_texcoords;

  friend class Singleton<ColoredTrailShader>;

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  // this class is a Singleton --- private c'tor and d'tor
  ColoredTrailShader();
  ~ColoredTrailShader() {};
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_COLORED_TRAIL_SHADER_HPP
