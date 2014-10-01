/*
 *  .file oglplus/enums/limit_query_names.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/oglplus/limit_query.txt' instead.
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace enums {
OGLPLUS_LIB_FUNC StrCRef ValueName_(
	LimitQuery*,
	GLenum value
)
#if (!OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OGLPLUS_IMPL_EVN_LIMITQUERY)
#define OGLPLUS_IMPL_EVN_LIMITQUERY
{
switch(value)
{
#if defined GL_MAX_3D_TEXTURE_SIZE
	case GL_MAX_3D_TEXTURE_SIZE: return StrCRef("MAX_3D_TEXTURE_SIZE");
#endif
#if defined GL_MAX_ARRAY_TEXTURE_LAYERS
	case GL_MAX_ARRAY_TEXTURE_LAYERS: return StrCRef("MAX_ARRAY_TEXTURE_LAYERS");
#endif
#if defined GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS
	case GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS: return StrCRef("MAX_ATOMIC_COUNTER_BUFFER_BINDINGS");
#endif
#if defined GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE
	case GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE: return StrCRef("MAX_ATOMIC_COUNTER_BUFFER_SIZE");
#endif
#if defined GL_MAX_CLIP_DISTANCES
	case GL_MAX_CLIP_DISTANCES: return StrCRef("MAX_CLIP_DISTANCES");
#endif
#if defined GL_MAX_COLOR_ATTACHMENTS
	case GL_MAX_COLOR_ATTACHMENTS: return StrCRef("MAX_COLOR_ATTACHMENTS");
#endif
#if defined GL_MAX_COLOR_TEXTURE_SAMPLES
	case GL_MAX_COLOR_TEXTURE_SAMPLES: return StrCRef("MAX_COLOR_TEXTURE_SAMPLES");
#endif
#if defined GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS
	case GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS: return StrCRef("MAX_COMBINED_ATOMIC_COUNTER_BUFFERS");
#endif
#if defined GL_MAX_COMBINED_ATOMIC_COUNTERS
	case GL_MAX_COMBINED_ATOMIC_COUNTERS: return StrCRef("MAX_COMBINED_ATOMIC_COUNTERS");
#endif
#if defined GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS
	case GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS: return StrCRef("MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
	case GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS: return StrCRef("MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_COMBINED_IMAGE_UNIFORMS
	case GL_MAX_COMBINED_IMAGE_UNIFORMS: return StrCRef("MAX_COMBINED_IMAGE_UNIFORMS");
#endif
#if defined GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS
	case GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS: return StrCRef("MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS");
#endif
#if defined GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS
	case GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS: return StrCRef("MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS
	case GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS: return StrCRef("MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
	case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: return StrCRef("MAX_COMBINED_TEXTURE_IMAGE_UNITS");
#endif
#if defined GL_MAX_COMBINED_UNIFORM_BLOCKS
	case GL_MAX_COMBINED_UNIFORM_BLOCKS: return StrCRef("MAX_COMBINED_UNIFORM_BLOCKS");
#endif
#if defined GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS
	case GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS: return StrCRef("MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_CUBE_MAP_TEXTURE_SIZE
	case GL_MAX_CUBE_MAP_TEXTURE_SIZE: return StrCRef("MAX_CUBE_MAP_TEXTURE_SIZE");
#endif
#if defined GL_MAX_DEPTH_TEXTURE_SAMPLES
	case GL_MAX_DEPTH_TEXTURE_SAMPLES: return StrCRef("MAX_DEPTH_TEXTURE_SAMPLES");
#endif
#if defined GL_MAX_DRAW_BUFFERS
	case GL_MAX_DRAW_BUFFERS: return StrCRef("MAX_DRAW_BUFFERS");
#endif
#if defined GL_MAX_DUAL_SOURCE_DRAW_BUFFERS
	case GL_MAX_DUAL_SOURCE_DRAW_BUFFERS: return StrCRef("MAX_DUAL_SOURCE_DRAW_BUFFERS");
#endif
#if defined GL_MAX_ELEMENTS_INDICES
	case GL_MAX_ELEMENTS_INDICES: return StrCRef("MAX_ELEMENTS_INDICES");
#endif
#if defined GL_MAX_ELEMENTS_VERTICES
	case GL_MAX_ELEMENTS_VERTICES: return StrCRef("MAX_ELEMENTS_VERTICES");
#endif
#if defined GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS
	case GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS: return StrCRef("MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS");
#endif
#if defined GL_MAX_FRAGMENT_ATOMIC_COUNTERS
	case GL_MAX_FRAGMENT_ATOMIC_COUNTERS: return StrCRef("MAX_FRAGMENT_ATOMIC_COUNTERS");
#endif
#if defined GL_MAX_FRAGMENT_IMAGE_UNIFORMS
	case GL_MAX_FRAGMENT_IMAGE_UNIFORMS: return StrCRef("MAX_FRAGMENT_IMAGE_UNIFORMS");
#endif
#if defined GL_MAX_FRAGMENT_INPUT_COMPONENTS
	case GL_MAX_FRAGMENT_INPUT_COMPONENTS: return StrCRef("MAX_FRAGMENT_INPUT_COMPONENTS");
#endif
#if defined GL_MAX_FRAGMENT_INTERPOLATION_OFFSET
	case GL_MAX_FRAGMENT_INTERPOLATION_OFFSET: return StrCRef("MAX_FRAGMENT_INTERPOLATION_OFFSET");
#endif
#if defined GL_MAX_FRAGMENT_UNIFORM_BLOCKS
	case GL_MAX_FRAGMENT_UNIFORM_BLOCKS: return StrCRef("MAX_FRAGMENT_UNIFORM_BLOCKS");
#endif
#if defined GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
	case GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: return StrCRef("MAX_FRAGMENT_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_FRAGMENT_UNIFORM_VECTORS
	case GL_MAX_FRAGMENT_UNIFORM_VECTORS: return StrCRef("MAX_FRAGMENT_UNIFORM_VECTORS");
#endif
#if defined GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS
	case GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS: return StrCRef("MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS");
#endif
#if defined GL_MAX_GEOMETRY_ATOMIC_COUNTERS
	case GL_MAX_GEOMETRY_ATOMIC_COUNTERS: return StrCRef("MAX_GEOMETRY_ATOMIC_COUNTERS");
#endif
#if defined GL_MAX_GEOMETRY_IMAGE_UNIFORMS
	case GL_MAX_GEOMETRY_IMAGE_UNIFORMS: return StrCRef("MAX_GEOMETRY_IMAGE_UNIFORMS");
#endif
#if defined GL_MAX_GEOMETRY_INPUT_COMPONENTS
	case GL_MAX_GEOMETRY_INPUT_COMPONENTS: return StrCRef("MAX_GEOMETRY_INPUT_COMPONENTS");
#endif
#if defined GL_MAX_GEOMETRY_OUTPUT_COMPONENTS
	case GL_MAX_GEOMETRY_OUTPUT_COMPONENTS: return StrCRef("MAX_GEOMETRY_OUTPUT_COMPONENTS");
#endif
#if defined GL_MAX_GEOMETRY_OUTPUT_VERTICES
	case GL_MAX_GEOMETRY_OUTPUT_VERTICES: return StrCRef("MAX_GEOMETRY_OUTPUT_VERTICES");
#endif
#if defined GL_MAX_GEOMETRY_SHADER_INVOCATIONS
	case GL_MAX_GEOMETRY_SHADER_INVOCATIONS: return StrCRef("MAX_GEOMETRY_SHADER_INVOCATIONS");
#endif
#if defined GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
	case GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS: return StrCRef("MAX_GEOMETRY_TEXTURE_IMAGE_UNITS");
#endif
#if defined GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS
	case GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS: return StrCRef("MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS");
#endif
#if defined GL_MAX_GEOMETRY_UNIFORM_BLOCKS
	case GL_MAX_GEOMETRY_UNIFORM_BLOCKS: return StrCRef("MAX_GEOMETRY_UNIFORM_BLOCKS");
#endif
#if defined GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
	case GL_MAX_GEOMETRY_UNIFORM_COMPONENTS: return StrCRef("MAX_GEOMETRY_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_IMAGE_SAMPLES
	case GL_MAX_IMAGE_SAMPLES: return StrCRef("MAX_IMAGE_SAMPLES");
#endif
#if defined GL_MAX_IMAGE_UNITS
	case GL_MAX_IMAGE_UNITS: return StrCRef("MAX_IMAGE_UNITS");
#endif
#if defined GL_MAX_INTEGER_SAMPLES
	case GL_MAX_INTEGER_SAMPLES: return StrCRef("MAX_INTEGER_SAMPLES");
#endif
#if defined GL_MAX_PATCH_VERTICES
	case GL_MAX_PATCH_VERTICES: return StrCRef("MAX_PATCH_VERTICES");
#endif
#if defined GL_MAX_PROGRAM_TEXEL_OFFSET
	case GL_MAX_PROGRAM_TEXEL_OFFSET: return StrCRef("MAX_PROGRAM_TEXEL_OFFSET");
#endif
#if defined GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET
	case GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET: return StrCRef("MAX_PROGRAM_TEXTURE_GATHER_OFFSET");
#endif
#if defined GL_MAX_RECTANGLE_TEXTURE_SIZE
	case GL_MAX_RECTANGLE_TEXTURE_SIZE: return StrCRef("MAX_RECTANGLE_TEXTURE_SIZE");
#endif
#if defined GL_MAX_RENDERBUFFER_SIZE
	case GL_MAX_RENDERBUFFER_SIZE: return StrCRef("MAX_RENDERBUFFER_SIZE");
#endif
#if defined GL_MAX_SAMPLE_MASK_WORDS
	case GL_MAX_SAMPLE_MASK_WORDS: return StrCRef("MAX_SAMPLE_MASK_WORDS");
#endif
#if defined GL_MAX_SAMPLES
	case GL_MAX_SAMPLES: return StrCRef("MAX_SAMPLES");
#endif
#if defined GL_MAX_SERVER_WAIT_TIMEOUT
	case GL_MAX_SERVER_WAIT_TIMEOUT: return StrCRef("MAX_SERVER_WAIT_TIMEOUT");
#endif
#if defined GL_MAX_SUBROUTINES
	case GL_MAX_SUBROUTINES: return StrCRef("MAX_SUBROUTINES");
#endif
#if defined GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS
	case GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS: return StrCRef("MAX_SUBROUTINE_UNIFORM_LOCATIONS");
#endif
#if defined GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS
	case GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS: return StrCRef("MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS");
#endif
#if defined GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS
	case GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS: return StrCRef("MAX_TESS_CONTROL_ATOMIC_COUNTERS");
#endif
#if defined GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS
	case GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS: return StrCRef("MAX_TESS_CONTROL_IMAGE_UNIFORMS");
#endif
#if defined GL_MAX_TESS_CONTROL_INPUT_COMPONENTS
	case GL_MAX_TESS_CONTROL_INPUT_COMPONENTS: return StrCRef("MAX_TESS_CONTROL_INPUT_COMPONENTS");
#endif
#if defined GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS
	case GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS: return StrCRef("MAX_TESS_CONTROL_OUTPUT_COMPONENTS");
#endif
#if defined GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS
	case GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS: return StrCRef("MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS");
#endif
#if defined GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS
	case GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS: return StrCRef("MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS");
#endif
#if defined GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS
	case GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS: return StrCRef("MAX_TESS_CONTROL_UNIFORM_BLOCKS");
#endif
#if defined GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS
	case GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS: return StrCRef("MAX_TESS_CONTROL_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS
	case GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS: return StrCRef("MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS");
#endif
#if defined GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS
	case GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS: return StrCRef("MAX_TESS_EVALUATION_ATOMIC_COUNTERS");
#endif
#if defined GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS
	case GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS: return StrCRef("MAX_TESS_EVALUATION_IMAGE_UNIFORMS");
#endif
#if defined GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS
	case GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS: return StrCRef("MAX_TESS_EVALUATION_INPUT_COMPONENTS");
#endif
#if defined GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS
	case GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS: return StrCRef("MAX_TESS_EVALUATION_OUTPUT_COMPONENTS");
#endif
#if defined GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS
	case GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS: return StrCRef("MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS");
#endif
#if defined GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS
	case GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS: return StrCRef("MAX_TESS_EVALUATION_UNIFORM_BLOCKS");
#endif
#if defined GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS
	case GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS: return StrCRef("MAX_TESS_EVALUATION_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_TESS_GEN_LEVEL
	case GL_MAX_TESS_GEN_LEVEL: return StrCRef("MAX_TESS_GEN_LEVEL");
#endif
#if defined GL_MAX_TESS_PATCH_COMPONENTS
	case GL_MAX_TESS_PATCH_COMPONENTS: return StrCRef("MAX_TESS_PATCH_COMPONENTS");
#endif
#if defined GL_MAX_TEXTURE_BUFFER_SIZE
	case GL_MAX_TEXTURE_BUFFER_SIZE: return StrCRef("MAX_TEXTURE_BUFFER_SIZE");
#endif
#if defined GL_MAX_TEXTURE_IMAGE_UNITS
	case GL_MAX_TEXTURE_IMAGE_UNITS: return StrCRef("MAX_TEXTURE_IMAGE_UNITS");
#endif
#if defined GL_MAX_TEXTURE_LOD_BIAS
	case GL_MAX_TEXTURE_LOD_BIAS: return StrCRef("MAX_TEXTURE_LOD_BIAS");
#endif
#if defined GL_MAX_TEXTURE_SIZE
	case GL_MAX_TEXTURE_SIZE: return StrCRef("MAX_TEXTURE_SIZE");
#endif
#if defined GL_MAX_TRANSFORM_FEEDBACK_BUFFERS
	case GL_MAX_TRANSFORM_FEEDBACK_BUFFERS: return StrCRef("MAX_TRANSFORM_FEEDBACK_BUFFERS");
#endif
#if defined GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS
	case GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS: return StrCRef("MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS");
#endif
#if defined GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS
	case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS: return StrCRef("MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS");
#endif
#if defined GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS
	case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS: return StrCRef("MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS");
#endif
#if defined GL_MAX_UNIFORM_BLOCK_SIZE
	case GL_MAX_UNIFORM_BLOCK_SIZE: return StrCRef("MAX_UNIFORM_BLOCK_SIZE");
#endif
#if defined GL_MAX_UNIFORM_BUFFER_BINDINGS
	case GL_MAX_UNIFORM_BUFFER_BINDINGS: return StrCRef("MAX_UNIFORM_BUFFER_BINDINGS");
#endif
#if defined GL_MAX_VARYING_COMPONENTS
	case GL_MAX_VARYING_COMPONENTS: return StrCRef("MAX_VARYING_COMPONENTS");
#endif
#if defined GL_MAX_VARYING_VECTORS
	case GL_MAX_VARYING_VECTORS: return StrCRef("MAX_VARYING_VECTORS");
#endif
#if defined GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS
	case GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS: return StrCRef("MAX_VERTEX_ATOMIC_COUNTER_BUFFERS");
#endif
#if defined GL_MAX_VERTEX_ATOMIC_COUNTERS
	case GL_MAX_VERTEX_ATOMIC_COUNTERS: return StrCRef("MAX_VERTEX_ATOMIC_COUNTERS");
#endif
#if defined GL_MAX_VERTEX_ATTRIBS
	case GL_MAX_VERTEX_ATTRIBS: return StrCRef("MAX_VERTEX_ATTRIBS");
#endif
#if defined GL_MAX_VERTEX_IMAGE_UNIFORMS
	case GL_MAX_VERTEX_IMAGE_UNIFORMS: return StrCRef("MAX_VERTEX_IMAGE_UNIFORMS");
#endif
#if defined GL_MAX_VERTEX_OUTPUT_COMPONENTS
	case GL_MAX_VERTEX_OUTPUT_COMPONENTS: return StrCRef("MAX_VERTEX_OUTPUT_COMPONENTS");
#endif
#if defined GL_MAX_VERTEX_STREAMS
	case GL_MAX_VERTEX_STREAMS: return StrCRef("MAX_VERTEX_STREAMS");
#endif
#if defined GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
	case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS: return StrCRef("MAX_VERTEX_TEXTURE_IMAGE_UNITS");
#endif
#if defined GL_MAX_VERTEX_UNIFORM_BLOCKS
	case GL_MAX_VERTEX_UNIFORM_BLOCKS: return StrCRef("MAX_VERTEX_UNIFORM_BLOCKS");
#endif
#if defined GL_MAX_VERTEX_UNIFORM_COMPONENTS
	case GL_MAX_VERTEX_UNIFORM_COMPONENTS: return StrCRef("MAX_VERTEX_UNIFORM_COMPONENTS");
#endif
#if defined GL_MAX_VERTEX_UNIFORM_VECTORS
	case GL_MAX_VERTEX_UNIFORM_VECTORS: return StrCRef("MAX_VERTEX_UNIFORM_VECTORS");
#endif
#if defined GL_MAX_VIEWPORT_DIMS
	case GL_MAX_VIEWPORT_DIMS: return StrCRef("MAX_VIEWPORT_DIMS");
#endif
#if defined GL_MAX_VIEWPORTS
	case GL_MAX_VIEWPORTS: return StrCRef("MAX_VIEWPORTS");
#endif
#if defined GL_MAX_COMPUTE_WORK_GROUP_COUNT
	case GL_MAX_COMPUTE_WORK_GROUP_COUNT: return StrCRef("MAX_COMPUTE_WORK_GROUP_COUNT");
#endif
#if defined GL_MAX_COMPUTE_WORK_GROUP_SIZE
	case GL_MAX_COMPUTE_WORK_GROUP_SIZE: return StrCRef("MAX_COMPUTE_WORK_GROUP_SIZE");
#endif
#if defined GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS
	case GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS: return StrCRef("MAX_COMPUTE_WORK_GROUP_INVOCATIONS");
#endif
#if defined GL_MAX_COMPUTE_SHARED_MEMORY_SIZE
	case GL_MAX_COMPUTE_SHARED_MEMORY_SIZE: return StrCRef("MAX_COMPUTE_SHARED_MEMORY_SIZE");
#endif
#if defined GL_MIN_FRAGMENT_INTERPOLATION_OFFSET
	case GL_MIN_FRAGMENT_INTERPOLATION_OFFSET: return StrCRef("MIN_FRAGMENT_INTERPOLATION_OFFSET");
#endif
#if defined GL_MIN_MAP_BUFFER_ALIGNMENT
	case GL_MIN_MAP_BUFFER_ALIGNMENT: return StrCRef("MIN_MAP_BUFFER_ALIGNMENT");
#endif
#if defined GL_MIN_PROGRAM_TEXEL_OFFSET
	case GL_MIN_PROGRAM_TEXEL_OFFSET: return StrCRef("MIN_PROGRAM_TEXEL_OFFSET");
#endif
#if defined GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET
	case GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET: return StrCRef("MIN_PROGRAM_TEXTURE_GATHER_OFFSET");
#endif
#if defined GL_MIN_SAMPLE_SHADING_VALUE
	case GL_MIN_SAMPLE_SHADING_VALUE: return StrCRef("MIN_SAMPLE_SHADING_VALUE");
#endif
	default:;
}
OGLPLUS_FAKE_USE(value);
return StrCRef();
}
#else
;
#endif
} // namespace enums
