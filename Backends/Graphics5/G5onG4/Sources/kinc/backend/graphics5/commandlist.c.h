#include <kinc/graphics4/graphics.h>
#include <kinc/graphics5/commandlist.h>
#include <kinc/graphics5/indexbuffer.h>
#include <kinc/graphics5/pipeline.h>
#include <kinc/graphics5/vertexbuffer.h>

#include <kinc/log.h>

#ifdef KORE_MICROSOFT
#include <malloc.h>
#endif

#define WRITE(type, value)                                                                                                                                     \
	*(type *)(&list->impl.commands[list->impl.commandIndex]) = value;                                                                                          \
	list->impl.commandIndex += sizeof(type);
#define READ(type, var)                                                                                                                                        \
	type var = *(type *)(&list->impl.commands[index]);                                                                                                         \
	index += sizeof(type);

typedef enum command {
	Clear,
	Draw,
	SetViewport,
	SetScissor,
	SetPipeline,
	SetVertexBuffers,
	SetIndexBuffer,
	SetRenderTargets,
	SetRenderTargetFace,
	DrawInstanced,
	SetTexture,
	SetTextureAdressing,
	SetTextureMagnificationFilter,
	SetTextureMinificationFilter,
	SetTextureMipmapFilter,
	SetImageTexture
} command_t;

void kinc_g5_command_list_init(kinc_g5_command_list_t *list) {}

void kinc_g5_command_list_destroy(kinc_g5_command_list_t *list) {}

void kinc_g5_command_list_begin(kinc_g5_command_list_t *list) {
	list->impl.commandIndex = 0;
}

void kinc_g5_command_list_end(kinc_g5_command_list_t *list) {}

void kinc_g5_command_list_clear(kinc_g5_command_list_t *list, struct kinc_g5_render_target *renderTarget, unsigned flags, unsigned color, float depth,
                                int stencil) {
	WRITE(command_t, Clear);
	WRITE(unsigned, flags);
	WRITE(unsigned, color);
	WRITE(float, depth);
	WRITE(int, stencil);
}

void kinc_g5_command_list_render_target_to_framebuffer_barrier(kinc_g5_command_list_t *list, struct kinc_g5_render_target *renderTarget) {}
void kinc_g5_command_list_framebuffer_to_render_target_barrier(kinc_g5_command_list_t *list, struct kinc_g5_render_target *renderTarget) {}
void kinc_g5_command_list_texture_to_render_target_barrier(kinc_g5_command_list_t *list, struct kinc_g5_render_target *renderTarget) {}
void kinc_g5_command_list_render_target_to_texture_barrier(kinc_g5_command_list_t *list, struct kinc_g5_render_target *renderTarget) {}

void kinc_g5_command_list_draw_indexed_vertices(kinc_g5_command_list_t *list) {
	kinc_g5_command_list_draw_indexed_vertices_from_to(list, 0, list->impl._indexCount);
}

void kinc_g5_command_list_draw_indexed_vertices_from_to(kinc_g5_command_list_t *list, int start, int count) {
	WRITE(command_t, Draw);
	WRITE(int, start);
	WRITE(int, count);
}

void kinc_g5_command_list_draw_indexed_vertices_instanced(kinc_g5_command_list_t *list, int instanceCount) {
	kinc_g5_command_list_draw_indexed_vertices_instanced_from_to(list, instanceCount, 0, list->impl._indexCount);
}
void kinc_g5_command_list_draw_indexed_vertices_instanced_from_to(kinc_g5_command_list_t *list, int instanceCount, int start, int count) {
	WRITE(command_t, DrawInstanced);
	WRITE(int, instanceCount);
	WRITE(int, start);
	WRITE(int, count);
}

void kinc_g5_command_list_viewport(kinc_g5_command_list_t *list, int x, int y, int width, int height) {
	WRITE(command_t, SetViewport);
	WRITE(int, x);
	WRITE(int, y);
	WRITE(int, width);
	WRITE(int, height);
}

void kinc_g5_command_list_scissor(kinc_g5_command_list_t *list, int x, int y, int width, int height) {
	WRITE(command_t, SetScissor);
	WRITE(int, x);
	WRITE(int, y);
	WRITE(int, width);
	WRITE(int, height);
}

void kinc_g5_command_list_disable_scissor(kinc_g5_command_list_t *list) {}

void kinc_g5_command_list_set_pipeline(kinc_g5_command_list_t *list, struct kinc_g5_pipeline *pipeline) {
	WRITE(command_t, SetPipeline);
	WRITE(kinc_g5_pipeline_t *, pipeline);
}

void kinc_g5_command_list_set_blend_constant(kinc_g5_command_list_t *list, float r, float g, float b, float a) {}

void kinc_g5_command_list_set_pipeline_layout(kinc_g5_command_list_t *list) {}

void kinc_g5_command_list_set_vertex_buffers(kinc_g5_command_list_t *list, struct kinc_g5_vertex_buffer **buffers, int *offsets, int count) {
	WRITE(command_t, SetVertexBuffers);
	WRITE(int, count);
	for (int i = 0; i < count; ++i) {
		WRITE(kinc_g5_vertex_buffer_t *, buffers[i]);
		if (offsets[i] != 0) {
			kinc_log(KINC_LOG_LEVEL_ERROR, "kinc_g5_command_list_set_vertex_buffers: offsets not supported");
		}
	}
}

void kinc_g5_command_list_set_index_buffer(kinc_g5_command_list_t *list, struct kinc_g5_index_buffer *buffer) {
	WRITE(command_t, SetIndexBuffer);
	WRITE(kinc_g5_index_buffer_t *, buffer);
	list->impl._indexCount = buffer->impl.myCount;
}

void kinc_g5_command_list_set_render_targets(kinc_g5_command_list_t *list, struct kinc_g5_render_target **targets, int count) {
	WRITE(command_t, SetRenderTargets);
	WRITE(int, count);
	for (int i = 0; i < count; ++i) {
		WRITE(kinc_g5_render_target_t *, targets[i]);
	}
}

void kinc_g5_command_list_upload_index_buffer(kinc_g5_command_list_t *list, struct kinc_g5_index_buffer *buffer) {}
void kinc_g5_command_list_upload_vertex_buffer(kinc_g5_command_list_t *list, struct kinc_g5_vertex_buffer *buffer) {}
void kinc_g5_command_list_upload_texture(kinc_g5_command_list_t *list, struct kinc_g5_texture *texture) {}

void kinc_g5_command_list_set_vertex_constant_buffer(kinc_g5_command_list_t *list, struct kinc_g5_constant_buffer *buffer, int offset, size_t size) {}

void kinc_g5_command_list_set_fragment_constant_buffer(kinc_g5_command_list_t *list, struct kinc_g5_constant_buffer *buffer, int offset, size_t size) {}

void kinc_g5_command_list_execute(kinc_g5_command_list_t *list) {
	int index = 0;
	while (index < list->impl.commandIndex) {
		READ(command_t, command);
		switch (command) {
		case Clear: {
			READ(unsigned, flags);
			READ(unsigned, color);
			READ(float, depth);
			READ(int, stencil);
			kinc_g4_clear(flags, color, depth, stencil);
			break;
		}
		case Draw: {
			READ(int, start);
			READ(int, count);
			kinc_g4_draw_indexed_vertices_from_to(start, count);
			break;
		}
		case SetViewport: {
			READ(int, x);
			READ(int, y);
			READ(int, width);
			READ(int, height);
			kinc_g4_viewport(x, y, width, height);
			break;
		}
		case SetScissor: {
			READ(int, x);
			READ(int, y);
			READ(int, width);
			READ(int, height);
			kinc_g4_scissor(x, y, width, height);
			break;
		}
		case SetPipeline: {
			READ(kinc_g5_pipeline_t *, pipeline);
			kinc_g4_set_pipeline(&pipeline->impl.pipe);
			break;
		}
		case SetVertexBuffers: {
			READ(int, count);
#ifdef KORE_MICROSOFT
			kinc_g4_vertex_buffer_t **buffers = (kinc_g4_vertex_buffer_t **)alloca(sizeof(kinc_g4_vertex_buffer_t *) * count);
#else
			kinc_g4_vertex_buffer_t *buffers[count];
#endif
			for (int i = 0; i < count; ++i) {
				READ(kinc_g5_vertex_buffer_t *, buffer);
				buffers[i] = &buffer->impl.buffer;
			}
			kinc_g4_set_vertex_buffers(buffers, count);
			break;
		}
		case SetIndexBuffer: {
			READ(kinc_g5_index_buffer_t *, buffer);
			kinc_g4_set_index_buffer(&buffer->impl.buffer);
			break;
		}
		case SetRenderTargets: {
			READ(int, count);
#ifdef KORE_MICROSOFT
			kinc_g4_render_target_t **buffers = (kinc_g4_render_target_t **)alloca(sizeof(kinc_g4_render_target_t *) * count);
#else
			kinc_g4_render_target_t *buffers[count];
#endif
			for (int i = 0; i < count; ++i) {
				READ(kinc_g5_render_target_t *, buffer);
				buffers[i] = &buffer->impl.target;
			}
			if (buffers[0]->contextId < 0) {
				if (count > 1) {
					kinc_log(KINC_LOG_LEVEL_ERROR, "Rendering to backbuffer and render targets at the same time is not supported");
				}
				kinc_g4_restore_render_target();
			}
			else {
				kinc_g4_set_render_targets(buffers, count);
			}
			break;
		}
		case SetRenderTargetFace: {
			READ(kinc_g5_render_target_t *, target);
			READ(int, face);
			kinc_g4_set_render_target_face(&target->impl.target, face);
			break;
		}
		case DrawInstanced: {
			READ(int, instanceCount);
			READ(int, start);
			READ(int, count);
			kinc_g4_draw_indexed_vertices_instanced_from_to(instanceCount, start, count);
			break;
		}
		case SetTexture: {
			READ(kinc_g5_texture_unit_t, unit);
			READ(kinc_g5_texture_t *, texture);
			kinc_g4_set_texture(unit.impl.unit, &texture->impl.texture);
			break;
		}
		case SetTextureAdressing: {
			READ(kinc_g5_texture_unit_t, unit);
			READ(kinc_g5_texture_direction_t, dir);
			READ(kinc_g5_texture_addressing_t, addressing);
			// assume for now that g5 and g4 match for the texture addressing and direction enums
			kinc_g4_set_texture_addressing(unit.impl.unit, dir, addressing);
			break;
		}
		case SetTextureMagnificationFilter: {
			READ(kinc_g5_texture_unit_t, unit);
			READ(kinc_g5_texture_filter_t, filter);
			kinc_g4_set_texture_magnification_filter(unit.impl.unit, filter);
			break;
		}
		case SetTextureMinificationFilter: {
			READ(kinc_g5_texture_unit_t, unit);
			READ(kinc_g5_texture_filter_t, filter);
			kinc_g4_set_texture_minification_filter(unit.impl.unit, filter);
			break;
		}
		case SetTextureMipmapFilter: {
			READ(kinc_g5_texture_unit_t, unit);
			READ(kinc_g5_texture_filter_t, filter);
			kinc_g4_set_texture_mipmap_filter(unit.impl.unit, filter);
			break;
		}
		case SetImageTexture: {
			READ(kinc_g5_texture_unit_t, unit);
			READ(kinc_g5_texture_t *, texture);
			kinc_g4_set_image_texture(unit.impl.unit, &texture->impl.texture);
			break;
		}
		default:
			kinc_log(KINC_LOG_LEVEL_ERROR, "Unknown command %i\n", command);
			return;
		}
	}
}

void kinc_g5_command_list_wait_for_execution_to_finish(kinc_g5_command_list_t *list) {
	kinc_g4_flush();
}

void kinc_g5_command_list_get_render_target_pixels(kinc_g5_command_list_t *list, struct kinc_g5_render_target *render_target, uint8_t *data) {
	kinc_log(KINC_LOG_LEVEL_ERROR, "kinc_g5_command_list_get_render_target_pixels not implemented");
}

void kinc_g5_command_list_set_texture_addressing(kinc_g5_command_list_t *list, kinc_g5_texture_unit_t unit, kinc_g5_texture_direction_t dir,
                                                 kinc_g5_texture_addressing_t addressing) {
	WRITE(command_t, SetTextureAdressing);
	WRITE(kinc_g5_texture_unit_t, unit);
	WRITE(kinc_g5_texture_direction_t, dir);
	WRITE(kinc_g5_texture_addressing_t, addressing);
}

void kinc_g5_command_list_set_texture_magnification_filter(kinc_g5_command_list_t *list, kinc_g5_texture_unit_t texunit, kinc_g5_texture_filter_t filter) {
	WRITE(command_t, SetTextureMagnificationFilter);
	WRITE(kinc_g5_texture_unit_t, texunit);
	WRITE(kinc_g5_texture_filter_t, filter);
}

void kinc_g5_command_list_set_texture_minification_filter(kinc_g5_command_list_t *list, kinc_g5_texture_unit_t texunit, kinc_g5_texture_filter_t filter) {
	WRITE(command_t, SetTextureMinificationFilter);
	WRITE(kinc_g5_texture_unit_t, texunit);
	WRITE(kinc_g5_texture_filter_t, filter);
}

void kinc_g5_command_list_set_texture_mipmap_filter(kinc_g5_command_list_t *list, kinc_g5_texture_unit_t texunit, kinc_g5_mipmap_filter_t filter) {
	WRITE(command_t, SetTextureMipmapFilter);
	WRITE(kinc_g5_texture_unit_t, texunit);
	WRITE(kinc_g5_mipmap_filter_t, filter);
}

void kinc_g5_command_list_set_render_target_face(kinc_g5_command_list_t *list, kinc_g5_render_target_t *texture, int face) {
	WRITE(command_t, SetRenderTargetFace);
	WRITE(kinc_g5_render_target_t *, texture);
	WRITE(int, face);
}

void kinc_g5_command_list_set_texture(kinc_g5_command_list_t *list, kinc_g5_texture_unit_t unit, kinc_g5_texture_t *texture) {
	WRITE(command_t, SetTexture);
	WRITE(kinc_g5_texture_unit_t, unit);
	WRITE(kinc_g5_texture_t *, texture);
}

void kinc_g5_command_list_set_image_texture(kinc_g5_command_list_t *list, kinc_g5_texture_unit_t unit, kinc_g5_texture_t *texture) {
	WRITE(command_t, SetImageTexture);
	WRITE(kinc_g5_texture_unit_t, unit);
	WRITE(kinc_g5_texture_t *, texture);
}

bool kinc_g5_command_list_init_occlusion_query(kinc_g5_command_list_t *list, unsigned *occlusionQuery) {
	return false;
}

void kinc_g5_command_list_delete_occlusion_query(kinc_g5_command_list_t *list, unsigned occlusionQuery) {}

void kinc_g5_command_list_render_occlusion_query(kinc_g5_command_list_t *list, unsigned occlusionQuery, int triangles) {}

bool kinc_g5_command_list_are_query_results_available(kinc_g5_command_list_t *list, unsigned occlusionQuery) {
	return false;
}

void kinc_g5_command_list_get_query_result(kinc_g5_command_list_t *list, unsigned occlusionQuery, unsigned *pixelCount) {}
