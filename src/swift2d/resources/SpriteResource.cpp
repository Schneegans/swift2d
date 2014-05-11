////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/resources/SpriteResource.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

SpriteResource::SpriteResource()
  : vs(nullptr)
  , fs(nullptr)
  , prog(nullptr)
  , rectangle(nullptr)
  , verts(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

SpriteResource::~SpriteResource() {
  if (vs) delete vs;
  if (fs) delete fs;
  if (prog) delete prog;
  if (rectangle) delete rectangle;
  if (verts) delete verts;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteResource::upload_to(RenderContext const& ctx) const {

  // ---------------------------------------------------------------------------
  // set the vertex shader source
  std::stringstream vs_source(
    "#version 330\n"
    "in vec2 position;"
    "uniform mat3 projection;"
    "uniform mat3 transform;"
    "out vec2 tex_coords;"
    "void main(void){"
    "  vec3 pos = projection * transform * vec3(position, 1.0);"
    "  tex_coords = position*0.5 + 0.5;"
    "  gl_Position = vec4(pos.xy, 0.0, 1.0);"
    "}"
  );
  vs = new oglplus::Shader(oglplus::ShaderType::Vertex);
  vs->Source(oglplus::GLSLSource::FromStream(vs_source));
  try {
    vs->Compile();
  } catch (oglplus::CompileError& e) {
    LOG_ERROR << "Failed to compile Vertex Shader!" << std::endl;
    LOG_ERROR << e.Log() << std::endl;
  }

  // set the fragment shader source
  std::stringstream fs_source(
    "#version 330\n"
    "in vec2 tex_coords;"
    "uniform sampler2D texture;"
    "out vec4 fragColor;"
    "void main(void){"
    "  vec4 color = texture2D(texture, tex_coords).rgba;"
    "  fragColor = vec4(color.rgb, color.a);"
    "}"
  );

  fs = new oglplus::Shader(oglplus::ShaderType::Fragment);
  fs->Source(oglplus::GLSLSource::FromStream(fs_source));
  try {
    fs->Compile();
  } catch (oglplus::CompileError& e) {
    LOG_ERROR << "Failed to compile Fragment Shader!" << std::endl;
    LOG_ERROR << e.Log() << std::endl;
  }

  // attach the shaders to the program
  prog = new oglplus::Program();
  prog->AttachShader(*vs);
  prog->AttachShader(*fs);
  // link and use it
  try {
    prog->Link();
  } catch (oglplus::LinkError& e) {
    LOG_ERROR << e.Log() << std::endl;
  }

  // ---------------------------------------------------------------------------
  // bind the VAO for the rectangle
  rectangle = new oglplus::VertexArray();
  rectangle->Bind();

  GLfloat rectangle_verts[8] = {
    -1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f
  };

  // bind the VBO for the rectangle vertices
  verts = new oglplus::Buffer();
  verts->Bind(oglplus::Buffer::Target::Array);
  // upload the data
  oglplus::Buffer::Data(oglplus::Buffer::Target::Array, 8, rectangle_verts);

  // setup the vertex attribs array for the vertices
  oglplus::VertexAttribArray vert_attr(*prog, "position");
  vert_attr.Setup<oglplus::Vec2f>().Enable();

  Logger::LOG_DEBUG << "uploaded! " << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void SpriteResource::draw(RenderContext const& ctx, math::mat3 const& transform) const {

  // upload to GPU if neccessary
  if (!prog) {
    upload_to(ctx);
  }

  rectangle->Bind();
  prog->Use();

  (*prog/"projection") = math::mat3(ctx.projection_matrix);
  (*prog/"transform") = math::mat3(transform);
  (*prog/"texture") = 0;

  ctx.gl.DrawArrays(oglplus::PrimitiveType::TriangleStrip, 0, 4);
}

}
