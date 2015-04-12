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

void Quad::upload() const {

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

    // bind the VAO for the rectangle_
    rectangle_ = new oglplus::VertexArray();
    rectangle_->Bind();

    oglplus::VertexArrayAttrib(0).Enable();
    oglplus::VertexArrayAttrib(0).Pointer(2, oglplus::DataType::Float, false, sizeof(math::vec2), (void const*)0);

    oglplus::NoVertexArray().Bind();
}

////////////////////////////////////////////////////////////////////////////////

void Quad::draw() const {
  // upload to GPU if neccessary
  if (!verts_) {
    upload();
  }

  rectangle_->Bind();
  ogl::Context::DrawArrays(oglplus::PrimitiveType::TriangleStrip, 0, 4);

  oglplus::NoVertexArray().Bind();
}

////////////////////////////////////////////////////////////////////////////////

void Quad::draw(unsigned instance_count) const {

  // upload to GPU if neccessary
  if (!verts_) {
    upload();
  }

  rectangle_->Bind();
  ogl::Context::DrawArraysInstanced(oglplus::PrimitiveType::TriangleStrip, 0, 4, instance_count);

  oglplus::NoVertexArray().Bind();
}

////////////////////////////////////////////////////////////////////////////////

}
