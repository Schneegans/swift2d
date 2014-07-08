////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/materials/Shader.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/materials/ShaderIncludes.hpp>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Shader::Shader(std::string const& v_source,
               std::string const& f_source,
               std::string const& g_source)
  : dirty_(true)
  , v_source_(v_source)
  , f_source_(f_source)
  , g_source_(g_source) {

  ShaderIncludes::instance()->process(v_source_);
  ShaderIncludes::instance()->process(f_source_);
  ShaderIncludes::instance()->process(g_source_);
}

////////////////////////////////////////////////////////////////////////////////

Shader::~Shader() {
}

////////////////////////////////////////////////////////////////////////////////

void Shader::upload_to(RenderContext const& ctx) const {

  dirty_ = false;

  // set the vertex shader source
  v_shader_.Source(v_source_);
  try {
    v_shader_.Compile();
  } catch (oglplus::CompileError& e) {
    LOG_ERROR << "Failed to compile Vertex Shader!" << std::endl;
    LOG_ERROR << e.Log() << std::endl;
  }

  // set the fragment shader source
  f_shader_.Source(f_source_);
  try {
    f_shader_.Compile();
  } catch (oglplus::CompileError& e) {
    LOG_ERROR << "Failed to compile Fragment Shader!" << std::endl;
    LOG_ERROR << e.Log() << std::endl;
  }

  // attach the shaders to the program
  program_.AttachShader(v_shader_);
  program_.AttachShader(f_shader_);

  if (g_source_ != "") {
    g_shader_.Source(g_source_);
    try {
      g_shader_.Compile();
    } catch (oglplus::CompileError& e) {
      LOG_ERROR << "Failed to compile Geometry Shader!" << std::endl;
      LOG_ERROR << e.Log() << std::endl;
    }

    program_.AttachShader(g_shader_);
  }

  // link and use it
  try {
    program_.Link();
  } catch (oglplus::LinkError& e) {
    LOG_ERROR << "Failed to link Shader!" << std::endl;
    LOG_ERROR << e.Log() << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Shader::use(RenderContext const& ctx) const {

  // upload to GPU if neccessary
  if (dirty_) {
    upload_to(ctx);
  }

  program_.Use();
}

////////////////////////////////////////////////////////////////////////////////

}
