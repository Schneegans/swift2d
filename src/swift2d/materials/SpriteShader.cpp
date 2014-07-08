////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/SpriteShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteShader::SpriteShader(int capabilities, std::string const& v_source,
                           std::string const& f_source)
  : Shader(v_source, f_source)
  , projection(get_uniform<math::mat3>("projection"))
  , transform(get_uniform<math::mat3>("transform"))
  , depth(get_uniform<float>("depth"))
  , parallax(get_uniform<float>("parallax"))
  , diffuse_tex(get_uniform<int>("diffuse_tex"))
  , diffuse(get_uniform<math::vec3>("diffuse"))
  , normal_tex(get_uniform<int>("normal_tex"))
  , normal_transform(get_uniform<math::mat3>("normal_transform"))
  , emit_tex(get_uniform<int>("emit_tex"))
  , emit(get_uniform<float>("emit"))
  , glow_tex(get_uniform<int>("glow_tex"))
  , glow(get_uniform<float>("glow"))
  , shinyness_tex(get_uniform<int>("shinyness_tex"))
  , shinyness(get_uniform<float>("shinyness"))
  , reflectivity_tex(get_uniform<int>("reflectivity_tex"))
  , reflectivity(get_uniform<float>("reflectivity")) {}

////////////////////////////////////////////////////////////////////////////////

}
