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

#include <memory>

namespace swift {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// shared pointer type definition ----------------------------------------------
class Shader;
typedef std::shared_ptr<Shader>       ShaderPtr;
typedef std::shared_ptr<const Shader> ConstShaderPtr;

// -----------------------------------------------------------------------------
class SWIFT_DLL Shader {

 ///////////////////////////////////////////////////////////////////////////////
 // ----------------------------------------------------------- public interface
 public:

  // ----------------------------------------------------- contruction interface

  // Creates a new component and returns a shared pointer.
  template <typename... Args>
  static ShaderPtr create(Args&& ... a) {
    return std::make_shared<Shader>(a...);
  }

  Shader(std::string const& v_source,
         std::string const& f_source,
         std::string const& g_source = "",
         std::vector<std::string> const& transform_feedback_varyings = {});

  // ------------------------------------------------------------ public methods

  // uses the Shader on the given context.
  void use(RenderContext const& ctx) const;

  template<typename T>
  void set_uniform(std::string name, T const& val) {
    oglplus::ProgramUniform<T>(program_, name).Set(val);
  }

  template<typename T>
  void set_uniform_array(std::string name, std::vector<T> const& val) {
    oglplus::ProgramUniform<T>(program_, name).Set(val.size(), val.data());
  }

  template<typename T>
  oglplus::Lazy<oglplus::Uniform<T>> get_uniform(std::string const& name) {
    return oglplus::Lazy<oglplus::Uniform<T>>(program_, name.c_str());
  }

 ///////////////////////////////////////////////////////////////////////////////
 // ---------------------------------------------------------- private interface
 private:
  void upload_to(RenderContext const& ctx) const;

  mutable oglplus::VertexShader     v_shader_;
  mutable oglplus::FragmentShader   f_shader_;
  mutable oglplus::GeometryShader   g_shader_;
  mutable oglplus::Program          program_;

  mutable bool dirty_;

  mutable std::string v_source_;
  mutable std::string f_source_;
  mutable std::string g_source_;
  std::vector<std::string> transform_feedback_varyings_;
};

// -----------------------------------------------------------------------------

}

#endif // SWIFT2D_SHADER_HPP
