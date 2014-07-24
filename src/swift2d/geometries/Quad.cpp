////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/Quad.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <oglplus/vertex_array.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

Quad::Quad()
  : rectangle_(nullptr)
  , verts_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

Quad::~Quad() {
  if (rectangle_) delete rectangle_;
  if (verts_)     delete verts_;
}

////////////////////////////////////////////////////////////////////////////////

void Quad::upload_to(RenderContext const& ctx) const {

  // bind the VAO for the rectangle_
  rectangle_ = new oglplus::VertexArray();
  rectangle_->Bind();

    std::vector<float> rect = {
      -1.0f, -1.0f,
      -1.0f,  1.0f,
       1.0f, -1.0f,
       1.0f,  1.0f
    };

    // bind the VBO for the rectangle_ vertices
    verts_ = new oglplus::Buffer();
    verts_->Bind(oglplus::Buffer::Target::Array);

    // upload the data
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, rect);
    oglplus::VertexArrayAttrib(0).Pointer(2, oglplus::DataType::Float, false, sizeof(math::vec2), (void const*)0);
    // oglplus::VertexArrayAttrib(0).Setup<oglplus::Vec2f>();

    oglplus::NoVertexArray().Bind();
}

////////////////////////////////////////////////////////////////////////////////

void Quad::draw(RenderContext const& ctx) const {

  // upload to GPU if neccessary
  if (!verts_) {
    upload_to(ctx);
  }

  rectangle_->Bind();
  oglplus::VertexArrayAttrib(0).Enable();
  ctx.gl.DrawArrays(oglplus::PrimitiveType::TriangleStrip, 0, 4);
}

////////////////////////////////////////////////////////////////////////////////

}
