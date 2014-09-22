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
  math::vec2  prev_u_texcoords; // last and prev_1
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

  // allocate GPU resources
  for (int i(0); i<2; ++i) {
    transform_feedbacks_.push_back(ogl::TransformFeedback());
    trail_buffers_.push_back(ogl::Buffer());
    trail_vaos_.push_back(ogl::VertexArray());

    trail_vaos_[i].Bind();
    trail_buffers_[i].Bind(ose::Array());

    for (long i(0); i<6; ++i) {
      ogl::VertexArrayAttrib(i).Pointer(2, ogl::DataType::Float, false, sizeof(TrailPoint), (void const*) (i*8));
      ogl::VertexArrayAttrib(i).Enable();
    }
  }

  emitter_buffer_ = new ogl::Buffer();
  emitter_vao_    = new ogl::VertexArray();

  emitter_vao_->Bind();
  emitter_buffer_->Bind(ose::Array());
  for (long i(0); i<6; ++i) {
    ogl::VertexArrayAttrib(i).Pointer(2, ogl::DataType::Float, false, sizeof(TrailPoint), (void const*) (i*8));
    ogl::VertexArrayAttrib(i).Enable();
  }
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::update_trails(
  std::vector<SerializedTrailEmitter> const& emitters,
  bool use_global_texcoords,
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
      data.front().prev_u_texcoords = math::vec2(0.f, 0.f);
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

  transform_feedbacks_[current_tf()].BeginPoints();
  {

    // spawn new particles -----------------------------------------------------
    for (auto const& emitter: emitters) {

      if (emitter.SpawnNewPoint) {
        math::vec2 time      (frame_time * 1000.0, total_time_ * 1000.0);
        float life           (emitter.Life * 1000.0);

        shader.spawn_count.            Set(1);
        shader.use_global_texcoords.   Set(use_global_texcoords ? 1 : 0);
        shader.position.               Set(emitter.LastPosition);
        shader.time_since_prev_spawns. Set(math::vec2(
                                            emitter.TimeSincePrev1Spawn * 1000.0,
                                            emitter.TimeSincePrev2Spawn * 1000.0
                                           )
                                          );
        shader.prev_1_position.        Set(emitter.Prev1Position);
        shader.prev_2_position.        Set(emitter.Prev2Position);
        shader.prev_3_position.        Set(emitter.Prev3Position);

        shader.time.                   Set(time);
        shader.life.                   Set(life);

        ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      }

    }

    // update existing particles -----------------------------------------------
    if (!first_draw) {
      shader.spawn_count.          Set(-1);
      shader.use_global_texcoords. Set(use_global_texcoords ? 1 : 0);

      ctx.gl.DrawTransformFeedback(
        ogl::PrimitiveType::Points, transform_feedbacks_[current_vb()]
      );
    }

  }

  transform_feedbacks_[current_tf()].End();

  ctx.gl.Disable(ogl::Capability::RasterizerDiscard);
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::draw_trails(
  std::vector<SerializedTrailEmitter> const& emitters,
  bool use_global_texcoords,
  RenderContext const& ctx) {

  std::vector<TrailPoint> emitter_points;

  for (auto const& emitter : emitters) {

    TrailPoint trail_point;

    trail_point.life = math::vec2(0.0, emitter.Life * 1000.0);

    if (emitter.LastPosition != emitter.Position) {
      auto next_dir(emitter.LastPosition - emitter.Prev1Position);

      trail_point.pos = emitter.Position + next_dir;

      if (use_global_texcoords) {
        trail_point.prev_u_texcoords = 1.0/emitter.Life *
                                       math::vec2(total_time_,
                                                  total_time_ -
                                                  emitter.TimeSinceLastSpawn);
      } else {
        trail_point.prev_u_texcoords = 1.0/emitter.Life *
                                       math::vec2(0.0, emitter.TimeSinceLastSpawn);
      }

      trail_point.prev_1_pos = emitter.Position;
      trail_point.prev_2_pos = emitter.LastPosition;
      trail_point.prev_3_pos = emitter.Prev1Position;

      emitter_points.push_back(trail_point);

      if (use_global_texcoords) {
        trail_point.prev_u_texcoords = 1.0/emitter.Life *
                                       math::vec2(total_time_ -
                                                  emitter.TimeSinceLastSpawn,
                                                  total_time_ -
                                                  emitter.TimeSincePrev1Spawn);
      } else {
        trail_point.prev_u_texcoords = 1.0/emitter.Life *
                                       math::vec2(emitter.TimeSinceLastSpawn,
                                                  emitter.TimeSincePrev1Spawn);
      }
      trail_point.pos = emitter.Position;
      trail_point.prev_1_pos = emitter.LastPosition;
      trail_point.prev_2_pos = emitter.Prev1Position;
      trail_point.prev_3_pos = emitter.Prev2Position;

      emitter_points.push_back(trail_point);
    } else {
      auto next_dir(emitter.Prev1Position - emitter.Prev2Position);

      trail_point.pos = emitter.Position + next_dir;

      if (use_global_texcoords) {
        trail_point.prev_u_texcoords = 1.0/emitter.Life *
                                       math::vec2(total_time_,
                                                  total_time_ -
                                                  emitter.TimeSincePrev1Spawn);
      } else {
        trail_point.prev_u_texcoords = 1.0/emitter.Life *
                                       math::vec2(0.0, emitter.TimeSincePrev1Spawn);
      }

      trail_point.prev_1_pos = emitter.Position;
      trail_point.prev_2_pos = emitter.Prev1Position;
      trail_point.prev_3_pos = emitter.Prev2Position;

      emitter_points.push_back(trail_point);
    }
  }

  emitter_vao_->Bind();
  emitter_buffer_->Bind(oglplus::Buffer::Target::Array);
  oglplus::Buffer::Data(oglplus::Buffer::Target::Array, emitter_points);

  ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, emitter_points.size());


  trail_vaos_[current_tf()].Bind();

  ctx.gl.DrawTransformFeedback(
    ogl::PrimitiveType::Points, transform_feedbacks_[current_tf()]
  );
}

////////////////////////////////////////////////////////////////////////////////

}
