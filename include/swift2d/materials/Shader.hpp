////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SWIFT2D_SHADER_HPP
#define SWIFT2D_SHADER_HPP

// includes  -------------------------------------------------------------------
#include <swift2d/graphics/RenderContext.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class Shader {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  Shader(std::string const& v_source,
         std::string const& f_source,
         std::string const& g_source = "");
  ~Shader();

  // uses the Shader on the given context.
  void use(RenderContext const& ctx) const;

  template<typename T>
  void set_uniform(std::string name, T const& val) {
    oglplus::ProgramUniform<T>(*program_, name).Set(val);
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& ctx) const;

  mutable oglplus::VertexShader*    v_shader_;
  mutable oglplus::FragmentShader*  f_shader_;
  mutable oglplus::GeometryShader*  g_shader_;
  mutable oglplus::Program*         program_;

  std::string v_source_;
  std::string f_source_;
  std::string g_source_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SHADER_HPP
