////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/resources/QuadResource.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

QuadResource::QuadResource()
  : rectangle_(nullptr)
  , verts_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

QuadResource::~QuadResource() {
  if (rectangle_) delete rectangle_;
  if (verts_)     delete verts_;
}

////////////////////////////////////////////////////////////////////////////////

void QuadResource::upload_to(RenderContext const& ctx) const {

  // ---------------------------------------------------------------------------
  // bind the VAO for the rectangle_
  rectangle_ = new oglplus::VertexArray();
  rectangle_->Bind();

  GLfloat rectangle_verts[8] = {
    -1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f
  };

  // bind the VBO for the rectangle_ vertices
  verts_ = new oglplus::Buffer();
  verts_->Bind(oglplus::Buffer::Target::Array);
  // upload the data
  oglplus::Buffer::Data(oglplus::Buffer::Target::Array, 8, rectangle_verts);

  // setup the vertex attribs array for the vertices
  oglplus::VertexAttribArray vert_attr(0);
  vert_attr.Setup<oglplus::Vec2f>().Enable();
}

////////////////////////////////////////////////////////////////////////////////

void QuadResource::draw(RenderContext const& ctx) const {

  // upload to GPU if neccessary
  if (!verts_) {
    upload_to(ctx);
  }

  rectangle_->Bind();
  ctx.gl.DrawArrays(oglplus::PrimitiveType::TriangleStrip, 0, 4);
}

////////////////////////////////////////////////////////////////////////////////

}
