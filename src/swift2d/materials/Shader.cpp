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

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Shader::Shader(std::string const& v_source, std::string const& f_source)
  : v_shader_(nullptr)
  , f_shader_(nullptr)
  , program_(nullptr)
  , v_source_(v_source)
  , f_source_(f_source) {}

////////////////////////////////////////////////////////////////////////////////

Shader::~Shader() {
  if (v_shader_) delete v_shader_;
  if (f_shader_) delete f_shader_;
  if (program_)  delete program_;
}

////////////////////////////////////////////////////////////////////////////////

void Shader::upload_to(RenderContext const& ctx) const {

  // set the vertex shader source
  v_shader_ = new oglplus::Shader(oglplus::ShaderType::Vertex);
  v_shader_->Source(v_source_);
  try {
    v_shader_->Compile();
  } catch (oglplus::CompileError& e) {
    LOG_ERROR << "Failed to compile Vertex Shader!" << std::endl;
    LOG_ERROR << e.Log() << std::endl;
  }

  // set the fragment shader source
  f_shader_ = new oglplus::Shader(oglplus::ShaderType::Fragment);
  f_shader_->Source(f_source_);
  try {
    f_shader_->Compile();
  } catch (oglplus::CompileError& e) {
    LOG_ERROR << "Failed to compile Fragment Shader!" << std::endl;
    LOG_ERROR << e.Log() << std::endl;
  }

  // attach the shaders to the program
  program_ = new oglplus::Program();
  program_->AttachShader(*v_shader_);
  program_->AttachShader(*f_shader_);

  // link and use it
  try {
    program_->Link();
  } catch (oglplus::LinkError& e) {
    LOG_ERROR << "Failed to link Shader!" << std::endl;
    LOG_ERROR << e.Log() << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////

void Shader::use(RenderContext const& ctx) const {

  // upload to GPU if neccessary
  if (!program_) {
    upload_to(ctx);
  }

  program_->Use();
}

////////////////////////////////////////////////////////////////////////////////

}
