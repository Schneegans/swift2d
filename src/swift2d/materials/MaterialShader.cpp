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

// includes  -------------------------------------------------------------------
#include <swift2d/materials/MaterialShader.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

MaterialShader::MaterialShader(int capabilities, std::string const& v_source,
                           std::string const& f_source)
  : Shader(v_source, f_source)
  , projection(get_uniform<math::mat3>("projection"))
  , transform(get_uniform<math::mat3>("transform"))
  , depth(get_uniform<float>("depth"))
  , parallax(get_uniform<float>("parallax"))
  , diffuse_tex(get_uniform<int>("diffuse_tex"))
  , diffuse(get_uniform<math::vec4>("diffuse"))
  , normal_tex(get_uniform<int>("normal_tex"))
  , normal_transform(get_uniform<math::mat3>("normal_transform"))
  , emit_tex(get_uniform<int>("emit_tex"))
  , emit(get_uniform<float>("emit"))
  , glow_tex(get_uniform<int>("glow_tex"))
  , glow(get_uniform<float>("glow"))
  , shinyness_tex(get_uniform<int>("shinyness_tex"))
  , shinyness(get_uniform<float>("shinyness"))
  , texcoord_offset_scale(get_uniform<math::vec4>("texcoord_offset_scale"))
  , time(get_uniform<float>("time")) {}

////////////////////////////////////////////////////////////////////////////////

}
