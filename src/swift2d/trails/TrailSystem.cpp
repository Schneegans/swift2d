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
  math::vec2  pos;
  math::vec2  life;
  math::vec2  prev_1_pos;
  math::vec2  prev_2_pos;
  math::vec2  prev_3_pos;
};

////////////////////////////////////////////////////////////////////////////////

TrailSystem::TrailSystem(int max_trail_points)
  : trails_to_spawn_()
  , transform_feedbacks_()
  , trail_vaos_()
  , trail_buffers_()
  , emitter_buffer_(nullptr)
  , emitter_vao_(nullptr)
  , ping_(true)
  , total_time_(0.0)
  , update_max_trail_points_(max_trail_points) {}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::set_max_trail_points(int max_trail_points) {
  update_max_trail_points_ = max_trail_points;
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::upload_to(RenderContext const& ctx) {

  auto t(ogl::DataType::Float);
  auto s(sizeof(TrailPoint));

  // allocate GPU resources
  for (int i(0); i<2; ++i) {
    transform_feedbacks_.push_back(ogl::TransformFeedback());
    trail_buffers_.push_back(ogl::Buffer());
    trail_vaos_.push_back(ogl::VertexArray());

    trail_vaos_[i].Bind();
    trail_buffers_[i].Bind(ose::Array());

    ogl::VertexArrayAttrib(0).Pointer(2, t, false, s, (void const*) 0);
    ogl::VertexArrayAttrib(1).Pointer(2, t, false, s, (void const*) 8);
    ogl::VertexArrayAttrib(2).Pointer(2, t, false, s, (void const*) 16);
    ogl::VertexArrayAttrib(3).Pointer(2, t, false, s, (void const*) 24);
    ogl::VertexArrayAttrib(4).Pointer(2, t, false, s, (void const*) 32);
  }

  emitter_buffer_ = new ogl::Buffer();
  emitter_vao_    = new ogl::VertexArray();

  emitter_vao_->Bind();
  emitter_buffer_->Bind(ose::Array());
  ogl::VertexArrayAttrib(0).Pointer(2, t, false, s, (void const*) 0);
  ogl::VertexArrayAttrib(1).Pointer(2, t, false, s, (void const*) 8);
  ogl::VertexArrayAttrib(2).Pointer(2, t, false, s, (void const*) 16);
  ogl::VertexArrayAttrib(3).Pointer(2, t, false, s, (void const*) 24);
  ogl::VertexArrayAttrib(4).Pointer(2, t, false, s, (void const*) 32);

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
      data.front().prev_1_pos = math::vec2(0.f, 0.f);
      data.front().prev_2_pos = math::vec2(0.f, 0.f);
      data.front().prev_3_pos = math::vec2(0.f, 0.f);
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

  auto& shader(TrailUpdateShader::get());
  shader.use(ctx);

  ogl::VertexArrayAttrib(0).Enable();
  ogl::VertexArrayAttrib(1).Enable();
  ogl::VertexArrayAttrib(2).Enable();
  ogl::VertexArrayAttrib(3).Enable();
  ogl::VertexArrayAttrib(4).Enable();

  transform_feedbacks_[current_tf()].BeginPoints();
  {

    // spawn new particles -----------------------------------------------------
    for (auto const& emitter: emitters) {

      if (emitter.SpawnNewPoint) {
        math::vec2 time      (frame_time * 1000.0, total_time_ * 1000.0);
        float life           (emitter.Life);

        shader.spawn_count.         Set(1);
        shader.position.            Set(emitter.LastPosition);
        shader.prev_1_position.     Set(emitter.Prev1Position);
        shader.prev_2_position.     Set(emitter.Prev2Position);
        shader.prev_3_position.     Set(emitter.Prev3Position);

        shader.time.                Set(time);
        shader.life.                Set(life);

        ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      }

    }

    // update existing particles -----------------------------------------------
    if (!first_draw) {
      shader.spawn_count.        Set(-1);

      ctx.gl.DrawTransformFeedback(
        ogl::PrimitiveType::Points, transform_feedbacks_[current_vb()]
      );
    }

  }

  transform_feedbacks_[current_tf()].End();

  ogl::VertexArrayAttrib(0).Disable();
  ogl::VertexArrayAttrib(1).Disable();
  ogl::VertexArrayAttrib(2).Disable();
  ogl::VertexArrayAttrib(3).Disable();
  ogl::VertexArrayAttrib(4).Disable();

  ctx.gl.Disable(ogl::Capability::RasterizerDiscard);
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::draw_trails(
  std::vector<SerializedTrailEmitter> const& emitters,
  RenderContext const& ctx) {

  std::vector<TrailPoint> emitter_points;

  for (auto const& emitter : emitters) {

    TrailPoint trail_point;

    trail_point.life = math::vec2();

    if (emitter.LastPosition != emitter.Position) {
      auto next_dir(emitter.LastPosition - emitter.Prev1Position);

      trail_point.pos = emitter.Position + next_dir;
      trail_point.prev_1_pos = emitter.Position;
      trail_point.prev_2_pos = emitter.LastPosition;
      trail_point.prev_3_pos = emitter.Prev1Position;

      emitter_points.push_back(trail_point);


      trail_point.pos = emitter.Position;
      trail_point.prev_1_pos = emitter.LastPosition;
      trail_point.prev_2_pos = emitter.Prev1Position;
      trail_point.prev_3_pos = emitter.Prev2Position;

      emitter_points.push_back(trail_point);
    } else {
      auto next_dir(emitter.Prev1Position - emitter.Prev2Position);

      trail_point.pos = emitter.Position + next_dir;
      trail_point.prev_1_pos = emitter.Position;
      trail_point.prev_2_pos = emitter.Prev1Position;
      trail_point.prev_3_pos = emitter.Prev2Position;

      emitter_points.push_back(trail_point);
    }
  }

  emitter_vao_->Bind();

  ogl::VertexArrayAttrib(0).Enable();
  ogl::VertexArrayAttrib(1).Enable();
  ogl::VertexArrayAttrib(2).Enable();
  ogl::VertexArrayAttrib(3).Enable();
  ogl::VertexArrayAttrib(4).Enable();

  emitter_buffer_->Bind(oglplus::Buffer::Target::Array);
  oglplus::Buffer::Data(oglplus::Buffer::Target::Array, emitter_points);

  ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, emitter_points.size());


  trail_vaos_[current_tf()].Bind();

  ogl::VertexArrayAttrib(0).Enable();
  ogl::VertexArrayAttrib(1).Enable();
  ogl::VertexArrayAttrib(2).Enable();
  ogl::VertexArrayAttrib(3).Enable();
  ogl::VertexArrayAttrib(4).Enable();

  ctx.gl.DrawTransformFeedback(
    ogl::PrimitiveType::Points, transform_feedbacks_[current_tf()]
  );

  ogl::VertexArrayAttrib(0).Disable();
  ogl::VertexArrayAttrib(1).Disable();
  ogl::VertexArrayAttrib(2).Disable();
  ogl::VertexArrayAttrib(3).Disable();
  ogl::VertexArrayAttrib(4).Disable();
}

////////////////////////////////////////////////////////////////////////////////

}
