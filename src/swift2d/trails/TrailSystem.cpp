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
#include <swift2d/trails/TrailSystemComponent.hpp>
#include <swift2d/trails/TrailUpdateShader.hpp>
#include <swift2d/textures/NoiseTexture.hpp>
#include <swift2d/math.hpp>
#include <swift2d/utils/Logger.hpp>
#include <swift2d/graphics/Pipeline.hpp>

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
    trail_vaos_.push_back(ogl::VertexArray());
    trail_buffers_.push_back(ogl::Buffer());

    trail_vaos_[i].Bind();
    trail_buffers_[i].Bind(ose::Array());

    for (long j(0); j<6; ++j) {
      ogl::VertexArrayAttrib(j).Enable();
      ogl::VertexArrayAttrib(j).Pointer(2, ogl::DataType::Float, false, sizeof(TrailPoint), (void const*)(j*2*sizeof(float)));
    }

    ogl::NoVertexArray().Bind();
  }

  emitter_buffer_ = new ogl::Buffer();
  emitter_vao_ = new ogl::VertexArray();

  emitter_vao_->Bind();
  emitter_buffer_->Bind(ose::Array());

  for (long j(0); j<6; ++j) {
    ogl::VertexArrayAttrib(j).Enable();
    ogl::VertexArrayAttrib(j).Pointer(2, ogl::DataType::Float, false, sizeof(TrailPoint), (void const*)(j*2*sizeof(float)));
  }

  ogl::NoVertexArray().Bind();
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::update_trails(
  std::vector<SerializedTrailEmitter> const& emitters,
  TrailSystemComponent::Serialized const& system,
  RenderContext const& ctx) {

  bool first_draw(false);

  // upload to GPU if neccessary
  if (trail_buffers_.empty()) {
    upload_to(ctx);
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
      ogl::Buffer::Data(ose::Array(), data, ose::StreamCopy());
    }

    update_max_trail_points_ = 0;
  }

  // get frame time
  double frame_time(ctx.pipeline->get_frame_time());
  double total_time(ctx.pipeline->get_total_time());

  // swap ping pong buffers
  ping_ = !ping_;

  ctx.gl.Enable(ogl::Capability::RasterizerDiscard);

  trail_vaos_         [current_vb()].Bind();
  transform_feedbacks_[current_tf()].Bind();
  trail_buffers_      [current_tf()].BindBase(ose::TransformFeedback(), 0);

  auto& shader(TrailUpdateShader::get());
  shader.use(ctx);
  math::vec2 time      (frame_time * 1000.0, total_time * 1000.0);
  shader.time.                   Set(time);
  shader.use_global_texcoords.   Set(system.UseGlobalTexCoords ? 1 : 0);
  shader.life.                   Set(system.Life * 1000.0);

  std::vector<math::vec2> positions0;
  std::vector<math::vec2> positions1;
  std::vector<math::vec2> positions2;
  std::vector<math::vec2> positions3;
  std::vector<math::vec2> times;

  transform_feedbacks_[current_tf()].BeginPoints();
  {

    // spawn new particles -----------------------------------------------------
    for (auto const& emitter: emitters) {
      if (emitter.SpawnNewPoint) {
        times.push_back(math::vec2(
          emitter.TimeSincePrev1Spawn * 1000.0,
          emitter.TimeSincePrev2Spawn * 1000.0
        ));
        positions0.push_back(emitter.LastPosition);
        positions1.push_back(emitter.Prev1Position);
        positions2.push_back(emitter.Prev2Position);
        positions3.push_back(emitter.Prev3Position);
      }
    }

    int index(0);

    while (index < positions0.size()) {
      int count(std::min(50, (int)positions0.size()-index));

      shader.spawn_count.Set(count);
      shader.time_since_prev_spawns. Set(std::vector<math::vec2>(times.begin() + index, times.begin() + index + count));
      shader.position.               Set(std::vector<math::vec2>(positions0.begin() + index, positions0.begin() + index + count));
      shader.prev_1_position.        Set(std::vector<math::vec2>(positions1.begin() + index, positions1.begin() + index + count));
      shader.prev_2_position.        Set(std::vector<math::vec2>(positions2.begin() + index, positions2.begin() + index + count));
      shader.prev_3_position.        Set(std::vector<math::vec2>(positions3.begin() + index, positions3.begin() + index + count));

      ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      index += count;
    }

    // update existing particles -----------------------------------------------
    if (!first_draw) {
      shader.spawn_count.Set(-1);
      ctx.gl.DrawTransformFeedback(
        ogl::PrimitiveType::Points, transform_feedbacks_[current_vb()]
      );
    }
  }

  transform_feedbacks_[current_tf()].End();

  ogl::DefaultTransformFeedback().Bind();
  ctx.gl.Disable(ogl::Capability::RasterizerDiscard);
  ogl::NoVertexArray().Bind();
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::draw_trails(
  std::vector<SerializedTrailEmitter> const& emitters,
  TrailSystemComponent::Serialized const& system,
  RenderContext const& ctx) {

  std::vector<TrailPoint> emitter_points;
  double total_time(ctx.pipeline->get_total_time());

  for (auto const& emitter : emitters) {

    TrailPoint trail_point;

    trail_point.life = math::vec2(0.0, system.Life * 1000.0);

    if (emitter.LastPosition != emitter.Position) {
      auto next_dir(emitter.LastPosition - emitter.Prev1Position);

      trail_point.pos = emitter.Position + next_dir;

      if (system.UseGlobalTexCoords) {
        trail_point.prev_u_texcoords = 1.0/system.Life *
                                       math::vec2(total_time,
                                                  total_time -
                                                  emitter.TimeSinceLastSpawn);
      } else {
        trail_point.prev_u_texcoords = 1.0/system.Life *
                                       math::vec2(0.0, emitter.TimeSinceLastSpawn);
      }

      trail_point.prev_1_pos = emitter.Position;
      trail_point.prev_2_pos = emitter.LastPosition;
      trail_point.prev_3_pos = emitter.Prev1Position;

      emitter_points.push_back(trail_point);

      if (system.UseGlobalTexCoords) {
        trail_point.prev_u_texcoords = 1.0/system.Life *
                                       math::vec2(total_time -
                                                  emitter.TimeSinceLastSpawn,
                                                  total_time -
                                                  emitter.TimeSincePrev1Spawn);
      } else {
        trail_point.prev_u_texcoords = 1.0/system.Life *
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

      if (system.UseGlobalTexCoords) {
        trail_point.prev_u_texcoords = 1.0/system.Life *
                                       math::vec2(total_time,
                                                  total_time -
                                                  emitter.TimeSincePrev1Spawn);
      } else {
        trail_point.prev_u_texcoords = 1.0/system.Life *
                                       math::vec2(0.0, emitter.TimeSincePrev1Spawn);
      }

      trail_point.prev_1_pos = emitter.Position;
      trail_point.prev_2_pos = emitter.Prev1Position;
      trail_point.prev_3_pos = emitter.Prev2Position;

      emitter_points.push_back(trail_point);
    }
  }

  emitter_buffer_->Bind(oglplus::Buffer::Target::Array);
  oglplus::Buffer::Data(oglplus::Buffer::Target::Array, emitter_points, ose::StreamDraw());

  emitter_vao_->Bind();
  ctx.gl.DrawArrays(ogl::PrimitiveType::Points, 0, emitter_points.size());

  trail_vaos_[current_tf()].Bind();
  ctx.gl.DrawTransformFeedback(
    ogl::PrimitiveType::Points, transform_feedbacks_[current_tf()]
  );
}

////////////////////////////////////////////////////////////////////////////////

}
