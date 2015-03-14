////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift2D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
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

struct GPUTrailSegment {
  math::vec2  pos;
  math::vec2  life;
  math::vec2  prev_u_texcoords; // last and prev_1
  math::vec2  prev_1_pos;
  math::vec2  prev_2_pos;
  math::vec2  prev_3_pos;
};

////////////////////////////////////////////////////////////////////////////////

TrailSystem::TrailSystem(int max_trail_points)
  : transform_feedbacks_()
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

void TrailSystem::spawn(
    std::vector<TrailSegment> const& end_segments,
    std::vector<TrailSegment> const& new_segments) {

  std::unique_lock<std::mutex> lock(mutex_);

  new_segments_.insert(new_segments_.begin(), new_segments.begin(), new_segments.end());
  end_segments_ping_ = end_segments;
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
      ogl::VertexArrayAttrib(j).Pointer(2, ogl::DataType::Float, false, sizeof(GPUTrailSegment), (void const*)(j*2*sizeof(float)));
    }

    ogl::NoVertexArray().Bind();
  }

  emitter_buffer_ = new ogl::Buffer();
  emitter_vao_    = new ogl::VertexArray();

  emitter_vao_->Bind();
  emitter_buffer_->Bind(ose::Array());

  for (long j(0); j<6; ++j) {
    ogl::VertexArrayAttrib(j).Enable();
    ogl::VertexArrayAttrib(j).Pointer(2, ogl::DataType::Float, false, sizeof(GPUTrailSegment), (void const*)(j*2*sizeof(float)));
  }

  ogl::NoVertexArray().Bind();
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::update_trails(
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

      std::vector<GPUTrailSegment> data(update_max_trail_points_);
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

  ogl::Context::Enable(ogl::Capability::RasterizerDiscard);

  trail_vaos_         [current_vb()].Bind();
  transform_feedbacks_[current_tf()].Bind();
  trail_buffers_      [current_tf()].BindBase(ose::TransformFeedback(), 0);

  auto& shader(TrailUpdateShader::get());
  shader.use();
  shader.time.                   Set(math::vec2(frame_time, total_time)*1000.0);
  shader.use_global_texcoords.   Set(system.UseGlobalTexCoords ? 1 : 0);

  std::vector<math::vec2> positions0;
  std::vector<math::vec2> positions1;
  std::vector<math::vec2> positions2;
  std::vector<math::vec2> positions3;
  std::vector<math::vec4> times;

  transform_feedbacks_[current_tf()].BeginPoints();
  {

    // spawn new particles -----------------------------------------------------
    {
      std::unique_lock<std::mutex> lock(mutex_);

      for (auto const& segment: new_segments_) {
        times.push_back(math::vec4(
          segment.TimeSincePrev1Spawn * 1000.0f,
          segment.TimeSincePrev2Spawn * 1000.0f,
          segment.Life * 1000.0f,
          segment.StartAge * 1000.f
        ));
        positions0.push_back(segment.Prev1Position);
        positions1.push_back(segment.Prev2Position);
        positions2.push_back(segment.Prev3Position);
        positions3.push_back(segment.Prev4Position);
      }

      new_segments_.clear();
      end_segments_pong_ = end_segments_ping_;
    }

    int index(0);

    while (index < positions0.size()) {
      std::size_t count(std::min(50, (int)positions0.size()-index));

      shader.spawn_count.     Set(count);
      shader.times.           Set(count, (const math::vec4*)&times[index]);
      shader.position.        Set(count, (const math::vec2*)&positions0[index]);
      shader.prev_1_position. Set(count, (const math::vec2*)&positions1[index]);
      shader.prev_2_position. Set(count, (const math::vec2*)&positions2[index]);
      shader.prev_3_position. Set(count, (const math::vec2*)&positions3[index]);

      ogl::Context::DrawArrays(ogl::PrimitiveType::Points, 0, 1);
      index += count;
    }

    // update existing particles -----------------------------------------------
    if (!first_draw) {
      shader.spawn_count.Set(-1);
      ogl::Context::DrawTransformFeedback(
        ogl::PrimitiveType::Points, transform_feedbacks_[current_vb()]
      );
    }
  }

  transform_feedbacks_[current_tf()].End();

  ogl::DefaultTransformFeedback().Bind();
  ogl::Context::Disable(ogl::Capability::RasterizerDiscard);
  ogl::NoVertexArray().Bind();
}

////////////////////////////////////////////////////////////////////////////////

void TrailSystem::draw_trails(
  TrailSystemComponent::Serialized const& system,
  RenderContext const& ctx) {

  std::vector<GPUTrailSegment> segments;
  double total_time(ctx.pipeline->get_total_time());

  {
    std::unique_lock<std::mutex> lock(mutex_);

    for (auto const& segment : end_segments_pong_) {

      GPUTrailSegment trail_point;

      math::vec2 start_age(segment.StartAge / segment.Life, segment.StartAge / segment.Life);
      trail_point.life = math::vec2(start_age.x(), segment.Life * 1000.0);


      if (segment.Prev1Position != segment.Position) {

        // create last segment
        auto next_dir(segment.Position - segment.Prev1Position);

        trail_point.pos = segment.Position + next_dir;

        if (system.UseGlobalTexCoords) {
          trail_point.prev_u_texcoords = 1.0/segment.Life *
                                         math::vec2(total_time,
                                                    total_time -
                                                    segment.TimeSinceLastSpawn);
          trail_point.prev_u_texcoords -= start_age;
        } else {
          trail_point.prev_u_texcoords = 1.0/segment.Life *
                                         math::vec2(0.0, segment.TimeSinceLastSpawn);
          trail_point.prev_u_texcoords += start_age;
        }

        trail_point.prev_1_pos = segment.Position;
        trail_point.prev_2_pos = segment.Prev1Position;
        trail_point.prev_3_pos = segment.Prev2Position;

        segments.push_back(trail_point);

        // create last but one segment
        if (system.UseGlobalTexCoords) {
          trail_point.prev_u_texcoords = 1.0/segment.Life *
                                         math::vec2(total_time -
                                                    segment.TimeSinceLastSpawn,
                                                    total_time -
                                                    segment.TimeSincePrev1Spawn);
          trail_point.prev_u_texcoords -= start_age;
        } else {
          trail_point.prev_u_texcoords = 1.0/segment.Life *
                                         math::vec2(segment.TimeSinceLastSpawn,
                                                    segment.TimeSincePrev1Spawn);
          trail_point.prev_u_texcoords += start_age;
        }
        trail_point.pos = segment.Position;
        trail_point.prev_1_pos = segment.Prev1Position;
        trail_point.prev_2_pos = segment.Prev2Position;
        trail_point.prev_3_pos = segment.Prev3Position;

        segments.push_back(trail_point);

      } else {
        // create only last segment
        auto next_dir(segment.Prev1Position - segment.Prev2Position);

        trail_point.pos = segment.Prev1Position + next_dir;

        if (system.UseGlobalTexCoords) {
          trail_point.prev_u_texcoords = 1.0/segment.Life *
                                         math::vec2(total_time,
                                                    total_time -
                                                    segment.TimeSincePrev1Spawn);
          trail_point.prev_u_texcoords -= start_age;
        } else {
          trail_point.prev_u_texcoords = 1.0/segment.Life *
                                         math::vec2(0.0, segment.TimeSincePrev1Spawn);
          trail_point.prev_u_texcoords += start_age;
        }

        trail_point.prev_1_pos = segment.Prev1Position;
        trail_point.prev_2_pos = segment.Prev2Position;
        trail_point.prev_3_pos = segment.Prev3Position;

        segments.push_back(trail_point);
      }
    }
  }

  emitter_buffer_->Bind(oglplus::Buffer::Target::Array);
  oglplus::Buffer::Data(oglplus::Buffer::Target::Array, segments, ose::StreamDraw());

  emitter_vao_->Bind();
  ogl::Context::DrawArrays(ogl::PrimitiveType::Points, 0, segments.size());

  trail_vaos_[current_tf()].Bind();
  ogl::Context::DrawTransformFeedback(
    ogl::PrimitiveType::Points, transform_feedbacks_[current_tf()]
  );
}

////////////////////////////////////////////////////////////////////////////////

}
