#pragma once

#include <hmm/HandmadeMath.h>
#include <vulkan/vulkan.h>

#include "font.h"
#include "graphics.h"
#include "ui.h"

typedef struct FontTexture {
    VkImage image;
    VkDeviceMemory image_memory;
    VkImageView image_view;
    VkSampler sampler;
} FontTexture;

typedef struct ScratchUIVertData {
    UIVert verts[MAX_UI_VERTS];
    uint16_t indices[MAX_UI_INDICES];
    uint32_t vert_count;
    uint32_t index_count;
} UIVertexData;

typedef struct FloatRect {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
} FloatRect;

typedef struct Buffer {
    VkBuffer handle;
    VkDeviceMemory memory;
    VkDeviceSize size;
} Buffer;

typedef struct FrameData {
    Buffer vertex_buffer;
    Buffer index_buffer;
    Buffer uniform_buffer;
} FrameData;

typedef struct Frame {
    VkCommandBuffer command_buffer;
    VkSemaphore image_ready_semaphore;
    VkSemaphore render_finished_semaphore;
    VkFence in_flight_fence;
    VkImage image;
    VkImageView image_view;
    FrameData ui_frame_data;
} Frame;

typedef struct {
    VkQueue queue;
    VkCommandPool command_pool;
    Frame frames[FRAME_COUNT];
    uint32_t frame_index;

    VkDescriptorPool descriptor_pool;
    VkDescriptorSet* ui_projection_sets;
    VkDescriptorSet* ui_texture_sets;

    FontTexture font_texture;

} Renderer;

Renderer* RendererInit(Arena* arena, Graphics* gfx, FontAtlas* font_atlas);
void RendererDestroy(Renderer* renderer, Graphics* gfx);
void RendererDrawUIFrame(Arena* scratch_arena, Renderer* renderer, Graphics* gfx,
                         UI* ui, FontAtlas* font_atlas, uint32_t frame_index);
