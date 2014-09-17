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
               std::string const& g_source,
               std::vector<std::string> const& transform_feedback_varyings)
  : dirty_(true)
  , v_source_(v_source)
  , f_source_(f_source)
  , g_source_(g_source)
  , transform_feedback_varyings_(transform_feedback_varyings) {}

////////////////////////////////////////////////////////////////////////////////

void Shader::upload_to(RenderContext const& ctx) const {

  dirty_ = false;

  ShaderIncludes::get().process(v_source_);
  ShaderIncludes::get().process(f_source_);
  ShaderIncludes::get().process(g_source_);

  // set the vertex shader source
  v_shader_.Source(v_source_);
  try {
    v_shader_.Compile();
  } catch (oglplus::CompileError& e) {
    LOG_ERROR << "Failed to compile Vertex Shader!" << std::endl;
    LOG_ERROR << e.Log() << std::endl;
  }

  program_.AttachShader(v_shader_);

  // set the fragment shader source
  if (f_source_ != "") {
    f_shader_.Source(f_source_);
    try {
      f_shader_.Compile();
    } catch (oglplus::CompileError& e) {
      LOG_ERROR << "Failed to compile Fragment Shader!" << std::endl;
      LOG_ERROR << e.Log() << std::endl;
    }

    program_.AttachShader(f_shader_);
  }

  // set the geometry shader source
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

  // add transform feedback varyings
  if (transform_feedback_varyings_.size() > 0) {
    program_.TransformFeedbackVaryings(
      transform_feedback_varyings_,
      oglplus::TransformFeedbackMode::InterleavedAttribs
    );
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
