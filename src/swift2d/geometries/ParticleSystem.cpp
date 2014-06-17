////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/geometries/ParticleSystem.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem()
  : shader_(
    // -------------------------------------------------------------------------
    R"(
      #version 330

      in vec2 position;
      // layout(location=0) in vec2 position;
      // layout(location=1) in float Age;

      // out float vertAge;

      void main(void) {
        gl_Position = vec4(position.xy, 0.0, 1.0);
        // vertAge = Age;
      }
    )",
    // -------------------------------------------------------------------------
    R"(
      #version 330

      // in  float geomAge;
      layout (location = 0) out vec4 fragColor;
      layout (location = 1) out vec4 fragNormal;
      layout (location = 2) out vec4 fragEmit;

      void main(void) {
        fragColor  = vec4(1, 0.5, 0.2, 1.0);
        fragNormal = vec4(0.5, 0.5, 0, fragColor.a);
        fragEmit   = vec4(1.0, 0, 0, fragColor.a);
      }
    )",
    // -------------------------------------------------------------------------
    R"(
      #version 330

      layout(points) in;
      layout(triangle_strip, max_vertices = 4) out;

      uniform mat3 transform;
      uniform mat3 projection;

      // in  float vertAge[];
      // out float geomAge;

      void main(void) {
        float yo[2] = float[2](0.5, -0.5);
        float xo[2] = float[2](0.5, -0.5);

        for(int j=0; j!=2; ++j) {
          for(int i=0; i!=2; ++i) {
            vec2 in_pos = gl_in[0].gl_Position.xy - vec2(xo[i], yo[j]);
            vec3 pos = projection * transform * vec3(in_pos, 1.0);
            gl_Position = vec4(pos.xy, 0.0, 1.0);
            // geomAge = vertAge[0];
            EmitVertex();
          }
        }
        EndPrimitive();
      }
    )"
  )
  , particles_(nullptr)
  , pos_buf_(nullptr)
  , age_buf_(nullptr) {}

////////////////////////////////////////////////////////////////////////////////

ParticleSystem::~ParticleSystem() {
  if (particles_) delete particles_;
  if (pos_buf_)   delete pos_buf_;
  if (age_buf_)   delete age_buf_;
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::update(double time) {

  if (positions_.size() < 100) {
    positions_.push_back(math::vec2(0.f, 0.f));
    // ages_.push_back(0);
  }

  for (int i(0); i<positions_.size(); ++i) {
    positions_[i] += math::vec2(0.2, 0);
    // ages_[i] += time;
  }

  if (particles_) {
    pos_buf_->Bind(oglplus::Buffer::Target::Array);
    oglplus::Buffer::Data(oglplus::Buffer::Target::Array, positions_);

    // age_buf_->Bind(oglplus::Buffer::Target::Array);
    // oglplus::Buffer::Data(oglplus::Buffer::Target::Array, ages_);
  }
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::upload_to(RenderContext const& ctx) const {
  shader_.use(ctx);

  ctx.gl.PointSize(20);

  particles_ = new oglplus::VertexArray();
  pos_buf_ = new oglplus::Buffer();
  // age_buf_ = new oglplus::Buffer();

  // bind the VAO for the particles
  particles_->Bind();

  // bind the VBO for the particle positions
  positions_.reserve(100);
  pos_buf_->Bind(oglplus::Buffer::Target::Array);
  {
      oglplus::Buffer::Data(oglplus::Buffer::Target::Array, positions_);
      oglplus::VertexAttribArray attr(*shader_.program_, "position");
      attr.Setup<GLfloat>(2);
      attr.Enable();
    }
  // oglplus::VertexAttribArray(0).Setup<oglplus::Vec2f>().Enable();

  // bind the VBO for the particle ages
  // ages_.reserve(100);
  // age_buf_->Bind(oglplus::Buffer::Target::Array);
  // oglplus::Buffer::Data(oglplus::Buffer::Target::Array, ages_);
  // oglplus::VertexAttribArray(1).Setup<float>().Enable();

  // oglplus::VertexArray::Unbind();
}

////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::draw(RenderContext const& ctx, math::mat3 const& object_transform) const {

  // upload to GPU if neccessary
  if (!particles_) {
    upload_to(ctx);
  }
  shader_.use(ctx);
  shader_.set_uniform("projection", ctx.projection_matrix);
  shader_.set_uniform("transform", object_transform);

  particles_->Bind();
  ctx.gl.DrawArrays(oglplus::PrimitiveType::Points, 0, positions_.size());
}

////////////////////////////////////////////////////////////////////////////////

}
