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
    "layout(location=0) in vec2 position;"
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
    "uniform sampler2D diffuse;"
    "uniform sampler2D normal;"
    "uniform bool with_normals;"
    ""
    "layout (location = 0) out vec4 fragColor;"
    "layout (location = 1) out vec4 fragNormal;"
    ""
    "void main(void){"
    "  if (!with_normals) {"
    "    fragColor = texture2D(diffuse, tex_coords);"
    "    fragNormal = vec4(0, 0, 0, 0);"
    "  } else {"
    "    fragColor = texture2D(diffuse, tex_coords);"
    "    fragNormal = texture2D(normal, tex_coords);"
    "  }"
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
  oglplus::VertexAttribArray vert_attr(0);
  vert_attr.Setup<oglplus::Vec2f>().Enable();
}

////////////////////////////////////////////////////////////////////////////////

void SpriteResource::draw(RenderContext const& ctx, math::mat3 const& transform, bool with_normals) const {

  // upload to GPU if neccessary
  if (!prog) {
    upload_to(ctx);
  }

  rectangle->Bind();
  prog->Use();

  (*prog/"diffuse") = 0;
  (*prog/"normal") = 1;
  (*prog/"projection") = math::mat3(ctx.projection_matrix);
  (*prog/"transform") = math::mat3(transform);

  (*prog/"with_normals") = (GLint)with_normals;

  ctx.gl.DrawArrays(oglplus::PrimitiveType::TriangleStrip, 0, 4);
}

}
