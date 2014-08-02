////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2014 Simon Schneegans & Felix Lauer                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// includes  -------------------------------------------------------------------
#include <swift2d/trails/TrailSystem.hpp>

#include <swift2d/physics/Physics.hpp>
#include <swift2d/trails/TrailUpdateShader.hpp>
#include <swift2d/textures/NoiseTexture.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>

#include <sstream>

namespace swift {

////////////////////////////////////////////////////////////////////////////////

struct TrailPoint {
  math::vec2 pos;
  math::vec2 life;
};

////////////////////////////////////////////////////////////////////////////////

TrailSystem::TrailSystem(int max_trail_points)
  : trails_to_spawn_()
  , transform_feedbacks_()
  , trail_vaos_()
  , trail_buffers_()
  , ping_(true)
  , total_time_(0.0)
  , update_max_trail_points_(max_trail_points) {}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::set_max_trail_points(int max_trail_points) {
  update_max_trail_points_ = max_trail_points;
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::upload_to(RenderContext const& ctx) {

  // allocate GPU resources
  for (int i(0); i<2; ++i) {
    transform_feedbacks_.push_back(ogl::TransformFeedback());
    trail_buffers_.push_back(ogl::Buffer());
    trail_vaos_.push_back(ogl::VertexArray());

    trail_vaos_[i].Bind();
    trail_buffers_[i].Bind(ose::Array());

    auto t(ogl::DataType::Float);
    auto s(sizeof(TrailPoint));

    ogl::VertexArrayAttrib(0).Pointer(2, t, false, s, (void const*) 0);
    ogl::VertexArrayAttrib(1).Pointer(2, t, false, s, (void const*) 8);
  }
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::update_trails(
  std::vector<SerializedTrailEmitter> const& emitters,
  RenderContext const& ctx) {

  bool first_draw(false);

  // upload to GPU if neccessary
  if (trail_buffers_.empty()) {
    upload_to(ctx);
    timer_.start();
    first_draw = true;
  }

  // update buffer size if necessary
  if (update_max_trail_points_ > 0) {
    for (int i(0); i<2; ++i) {
      trail_buffers_[i].Bind(ose::Array());

      std::vector<TrailPoint> data(update_max_trail_points_);
      data.front().pos  = math::vec2(0.f, 0.f);
      data.front().life = math::vec2(0.f, 0.f);
      ogl::Buffer::Data(ose::Array(), data, ose::DynamicDraw());
    }

    update_max_trail_points_ = 0;
  }

  // get frame time
  double frame_time(timer_.reset());
  total_time_ += frame_time;

  // swap ping pong buffers
  ping_ = !ping_;

  ctx.gl.Enable(ogl::Capability::RasterizerDiscard);

  trail_vaos_         [current_vb()].Bind();
  transform_feedbacks_[current_tf()].Bind();
  trail_buffers_      [current_tf()].BindBase(ose::TransformFeedback(), 0);

  auto shader(TrailUpdateShader::instance());
  shader->use(ctx);

  ogl::VertexArrayAttrib(0).Enable();
  ogl::VertexArrayAttrib(1).Enable();

  transform_feedbacks_[current_tf()].BeginPoints();
  {

    // spawn new particles -----------------------------------------------------
    for (auto const& emitter: emitters) {

      // calculate spawn count
      int spawn_count(0);

      trails_to_spawn_[emitter.Self] += emitter.Density * frame_time;
      spawn_count = trails_to_spawn_[emitter.Self];
      trails_to_spawn_[emitter.Self] -= spawn_count;

      if (spawn_count > 0) {
        math::vec2 time           (frame_time * 1000.0, total_time_ * 1000.0);
        float life                (emitter.Life);

        shader->spawn_count.      Set(spawn_count);
        shader->transform.        Set(emitter.WorldTransform);

        shader->time.             Set(time);
        shader->life.             Set(life);

        ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      }

    }

    // update existing particles -----------------------------------------------
    if (!first_draw) {
      shader->spawn_count.        Set(-1);

      ctx.gl.DrawTransformFeedback(
        ogl::PrimitiveType::Points, transform_feedbacks_[current_vb()]
      );
    }

  }

  transform_feedbacks_[current_tf()].End();

  ogl::VertexArrayAttrib(0).Disable();
  ogl::VertexArrayAttrib(1).Disable();

  ctx.gl.Disable(ogl::Capability::RasterizerDiscard);
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::draw_trails(RenderContext const& ctx) {
  trail_vaos_[current_tf()].Bind();

  ogl::VertexArrayAttrib(0).Enable();
  ogl::VertexArrayAttrib(1).Enable();

  ctx.gl.DrawTransformFeedback(
    ogl::PrimitiveType::LineStrip, transform_feedbacks_[current_tf()]
  );

  ogl::VertexArrayAttrib(0).Disable();
  ogl::VertexArrayAttrib(1).Disable();
}

////////////////////////////////////////////////////////////////////////////////

}