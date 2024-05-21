#include "renderer.h"

VkDeviceSize GetAlignedSize(Graphics *gfx, VkDeviceSize size) {
    VkDeviceSize alignment = gfx->gpu.min_device_pointer_alignment;
    return (size + alignment - 1) & ~(alignment - 1);
}

int32_t FindMemoryType(Graphics *gfx, uint32_t memory_type_filter, VkMemoryPropertyFlags property_flags) {
    VkPhysicalDeviceMemoryProperties mem_props = gfx->gpu.memory_properties;

    int32_t memory_type_index = -1;
    for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++) {
        if ((memory_type_filter & (1 << i)) && (mem_props.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
            memory_type_index = i;
            break;
        }
    }
    if (memory_type_index == -1) {
        SDL_Log("Failed to find suitable memory type\n");
        return -1;
    }

    return memory_type_index;
}

bool GetBuffer(Graphics *gfx,
               VkDeviceSize size,
               VkBufferUsageFlags usage_flags,
               VkMemoryPropertyFlags property_flags,
               Buffer *out_buffer) {
    VkBufferCreateInfo buffer_info = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage_flags,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VkResult result = gfx->vkd.vkCreateBuffer(gfx->device, &buffer_info, NULL, &out_buffer->handle);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to create buffer\n");
        return false;
    }

    VkMemoryRequirements mem_reqs;
    gfx->vkd.vkGetBufferMemoryRequirements(gfx->device, out_buffer->handle, &mem_reqs);

    uint32_t memory_type_index = FindMemoryType(gfx, mem_reqs.memoryTypeBits, property_flags);
    if (memory_type_index == -1) {
        SDL_Log("Failed to find suitable memory type for buffer\n");
        return false;
    }

    VkMemoryAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = mem_reqs.size,
        .memoryTypeIndex = memory_type_index,
    };

    result = gfx->vkd.vkAllocateMemory(gfx->device, &alloc_info, NULL, &out_buffer->memory);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to allocate buffer memory\n");
        return false;
    }

    result = gfx->vkd.vkBindBufferMemory(gfx->device, out_buffer->handle, out_buffer->memory, 0);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to bind buffer memory\n");
        return false;
    }

    return true;
}

VkCommandBuffer BeginSingleCommand(Graphics *gfx, Renderer *renderer) {
    Vkd vkd = gfx->vkd;

    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool = renderer->command_pool,
        .commandBufferCount = 1,
    };

    VkCommandBuffer command_buffer;
    VkResult result = vkd.vkAllocateCommandBuffers(gfx->device, &alloc_info, &command_buffer);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to allocate command buffer");
        return VK_NULL_HANDLE;
    }

    VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    result = vkd.vkBeginCommandBuffer(command_buffer, &begin_info);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to begin command buffer");
        return VK_NULL_HANDLE;
    }

    return command_buffer;
}

void EndSingleCommand(Graphics *gfx, Renderer *renderer, VkCommandBuffer command_buffer) {
    Vkd vkd = gfx->vkd;

    VkResult result = vkd.vkEndCommandBuffer(command_buffer);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to end command buffer");
        return;
    }

    VkSubmitInfo submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &command_buffer,
    };

    result = vkd.vkQueueSubmit(renderer->queue, 1, &submit_info, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to submit command buffer");
        return;
    }

    vkd.vkQueueWaitIdle(renderer->queue);
    vkd.vkFreeCommandBuffers(gfx->device, renderer->command_pool, 1, &command_buffer);
}

void CmdCopyBuffer(Graphics *gfx, Renderer *renderer, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size) {
    Vkd vkd = gfx->vkd;

    VkCommandBuffer command_buffer = BeginSingleCommand(gfx, renderer);
    if (command_buffer == VK_NULL_HANDLE) {
        SDL_Log("Failed to begin single command");
        return;
    }

    VkBufferCopy copy_region = {.size = size};
    vkd.vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

    EndSingleCommand(gfx, renderer, command_buffer);
}

void CmdCopyBufferToImage(Graphics *gfx, Renderer *renderer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    Vkd vkd = gfx->vkd;

    VkCommandBuffer command_buffer = BeginSingleCommand(gfx, renderer);
    if (command_buffer == VK_NULL_HANDLE) {
        SDL_Log("Failed to begin single command");
        return;
    }

    VkBufferImageCopy region = {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .mipLevel = 0,
                             .baseArrayLayer = 0,
                             .layerCount = 1},
        .imageOffset = {0, 0, 0},
        .imageExtent = {width, height, 1},
    };

    vkd.vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    EndSingleCommand(gfx, renderer, command_buffer);
}

void TransitionImageForPresent(Graphics *gfx, VkCommandBuffer command_buffer, VkImage image) {
    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        .image = image,
        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .baseMipLevel = 0,
                             .levelCount = 1,
                             .baseArrayLayer = 0,
                             .layerCount = 1},
    };
    gfx->vkd.vkCmdPipelineBarrier(command_buffer,
                             VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                             0,
                             0, VK_NULL_HANDLE,
                             0, VK_NULL_HANDLE,
                             1, &barrier);
};

void TransitionImageForRendering(Graphics *gfx, VkCommandBuffer command_buffer, VkImage image) {
    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .image = image,
        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .baseMipLevel = 0,
                             .levelCount = 1,
                             .baseArrayLayer = 0,
                             .layerCount = 1},
    };
    gfx->vkd.vkCmdPipelineBarrier(command_buffer,
                             VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             0,
                             0, VK_NULL_HANDLE,
                             0, VK_NULL_HANDLE,
                             1, &barrier);
};

void CmdTransitionImageLayout(Graphics *gfx, Renderer *renderer, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout) {
    VkCommandBuffer command_buffer = BeginSingleCommand(gfx, renderer);
    if (command_buffer == VK_NULL_HANDLE) {
        SDL_Log("Failed to begin single command");
        return;
    }

    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = old_layout,
        .newLayout = new_layout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = image,
        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .baseMipLevel = 0,
                             .levelCount = 1,
                             .baseArrayLayer = 0,
                             .layerCount = 1},
    };
    VkPipelineStageFlags source_stage;
    VkPipelineStageFlags destination_stage;

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        SDL_Log("Unsupported layout transition");
        return;
    }

    // Despite the name, this command can be used for transitioning layouts and queue ownership as well as syncing
    gfx->vkd.vkCmdPipelineBarrier(command_buffer,
                             source_stage, destination_stage,
                             0,
                             0, VK_NULL_HANDLE,
                             0, VK_NULL_HANDLE,
                             1, &barrier);

    EndSingleCommand(gfx, renderer, command_buffer);
}

bool GetImage(Graphics *gfx,
              Renderer *renderer,
              VkDeviceSize size,
              void *pixels,
              uint32_t width, uint32_t height,
              VkFormat format,
              VkImage *out_image,
              VkDeviceMemory *out_image_memory,
              VkImageView *out_image_view) {
    Vkd vkd = gfx->vkd;

    Buffer stg_buf;
    bool result = GetBuffer(gfx, size,
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                            &stg_buf);

    void *data;
    result = vkd.vkMapMemory(gfx->device, stg_buf.memory, 0, size, 0, &data);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to map image staging buffer memory");
        return false;
    }
    memcpy(data, pixels, size);
    vkd.vkUnmapMemory(gfx->device, stg_buf.memory);

    // Image + image memory
    VkImageCreateInfo image_info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = format,
        .extent = {width, height, 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    result = vkd.vkCreateImage(gfx->device, &image_info, NULL, out_image);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to create image");
        return false;
    }

    VkMemoryRequirements mem_reqs;
    vkd.vkGetImageMemoryRequirements(gfx->device, *out_image, &mem_reqs);
    int32_t memory_type_index = FindMemoryType(gfx, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (memory_type_index == -1) {
        SDL_Log("Failed to find suitable memory type for image");
        return false;
    }

    VkMemoryAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = mem_reqs.size,
        .memoryTypeIndex = memory_type_index,
    };
    result = vkd.vkAllocateMemory(gfx->device, &alloc_info, NULL, out_image_memory);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to allocate image memory");
        return false;
    }
    result = vkd.vkBindImageMemory(gfx->device, *out_image, *out_image_memory, 0);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to bind image memory");
        return false;
    }

    // Copy staging buffer to image
    CmdTransitionImageLayout(gfx, renderer, *out_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CmdCopyBufferToImage(gfx, renderer, stg_buf.handle, *out_image, width, height);
    CmdTransitionImageLayout(gfx, renderer, *out_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkd.vkDestroyBuffer(gfx->device, stg_buf.handle, NULL);
    vkd.vkFreeMemory(gfx->device, stg_buf.memory, NULL);

    // Image view
    VkImageViewCreateInfo view_info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = *out_image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                             .baseMipLevel = 0,
                             .levelCount = 1,
                             .baseArrayLayer = 0,
                             .layerCount = 1},
    };
    result = vkd.vkCreateImageView(gfx->device, &view_info, NULL, out_image_view);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to create image view");
        return false;
    }

    return true;
}

bool GetImageSampler(Graphics *gfx, Renderer *renderer, VkSampler *out_sampler) {
    Vkd vkd = gfx->vkd;

    VkSamplerCreateInfo sampler_info = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = VK_FILTER_NEAREST,
        .minFilter = VK_FILTER_NEAREST,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
        .anisotropyEnable = VK_FALSE,
        .maxAnisotropy = 0,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .mipLodBias = 0.0f,
        .minLod = 0.0f,
        .maxLod = 0.0f,
    };

    VkResult result = vkd.vkCreateSampler(gfx->device, &sampler_info, NULL, out_sampler);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to create sampler");
        return false;
    }

    return true;
}

void LoadFontTexture(Graphics *gfx, Renderer *renderer, FontAtlas *font_atlas) {
    FontTexture *font_texture = &renderer->font_texture;

    VkDeviceSize size = font_atlas->width * font_atlas->height * 4;

    bool success = GetImage(gfx, renderer, size, font_atlas->pixels,
                            font_atlas->width, font_atlas->height,
                            VK_FORMAT_R8_UNORM,
                            &font_texture->image,
                            &font_texture->image_memory,
                            &font_texture->image_view);
    if (!success) {
        ExitWithError("Failed to get font image");
    }

    success = GetImageSampler(gfx, renderer, &font_texture->sampler);
    if (!success) {
        ExitWithError("Failed to get font sampler");
    }
}

VkDescriptorSet *GetUIDescriptorSets(Arena *arena, Graphics *gfx, VkDescriptorPool pool, VkDescriptorSetLayout layout) {
    VkDescriptorSetLayout *layouts = ArenaPushArray(arena, VkDescriptorSetLayout, FRAME_COUNT);
    for (uint32_t i = 0; i < FRAME_COUNT; i++) {
        layouts[i] = layout;
    }

    VkDescriptorSet *sets = ArenaPushArray(arena, VkDescriptorSet, FRAME_COUNT);
    VkDescriptorSetAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = pool,
        .descriptorSetCount = FRAME_COUNT,
        .pSetLayouts = layouts,
    };

    VkResult result = gfx->vkd.vkAllocateDescriptorSets(gfx->device, &alloc_info, sets);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to allocate descriptor sets\n");
        return VK_NULL_HANDLE;
    }

    return sets;
}

VkDescriptorPool GetDescriptorPool(Graphics *gfx) {
    VkDescriptorPoolSize pool_sizes[] = {
        {.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
         .descriptorCount = FRAME_COUNT},
        {.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         .descriptorCount = FRAME_COUNT},
    };
    VkDescriptorPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .poolSizeCount = 2,
        .pPoolSizes = pool_sizes,
        .maxSets = FRAME_COUNT * 2};

    VkDescriptorPool pool;
    VkResult result = gfx->vkd.vkCreateDescriptorPool(gfx->device, &pool_info, NULL, &pool);
    assert(result == VK_SUCCESS);

    return pool;
}

void SetUITexture(Graphics *gfx, Renderer *renderer, VkDescriptorSet *descriptor_set) {
    Vkd vkd = gfx->vkd;

    VkDescriptorImageInfo image_info = {
        .sampler = renderer->font_texture.sampler,
        .imageView = renderer->font_texture.image_view,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};

    VkWriteDescriptorSet write = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = *descriptor_set,
        .dstBinding = 0,
        .dstArrayElement = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .pImageInfo = &image_info};

    vkd.vkUpdateDescriptorSets(gfx->device, 1, &write, 0, NULL);
}

void PushUIQuad(FontAtlas *font_atlas, UIVertexData *data, FloatRect dst, FloatRect src, mu_Color color) {
    f32 tex_left = (f32)src.x / font_atlas->width;
    f32 tex_top = (f32)src.y / font_atlas->height;
    f32 tex_right = (f32)(src.x + src.w) / font_atlas->width;
    f32 tex_bottom = (f32)(src.y + src.h) / font_atlas->height;

    uint32_t vert_offset = data->vert_count;
    uint32_t index_offset = data->index_count;

    data->verts[vert_offset] = (UIVert){
        .pos = {dst.x, (float)dst.y},
        .uv = {tex_left, tex_top},
        .colour = {color.r, color.g, color.b, color.a}};
    data->verts[vert_offset + 1] = (UIVert){
        .pos = {dst.x + dst.w, dst.y},
        .uv = {tex_right, tex_top},
        .colour = {color.r, color.g, color.b, color.a}};
    data->verts[vert_offset + 2] = (UIVert){
        .pos = {dst.x + dst.w, dst.y + dst.h},
        .uv = {tex_right, tex_bottom},
        .colour = {color.r, color.g, color.b, color.a}};
    data->verts[vert_offset + 3] = (UIVert){
        .pos = {dst.x, dst.y + dst.h},
        .uv = {tex_left, tex_bottom},
        .colour = {color.r, color.g, color.b, color.a}};

    data->indices[index_offset] = vert_offset;
    data->indices[index_offset + 1] = vert_offset + 1;
    data->indices[index_offset + 2] = vert_offset + 2;
    data->indices[index_offset + 3] = vert_offset + 2;
    data->indices[index_offset + 4] = vert_offset + 3;
    data->indices[index_offset + 5] = vert_offset;

    data->vert_count += 4;
    data->index_count += 6;
}

void DrawUIText(FontAtlas *font_atlas, UIVertexData *data,
                const char *text, mu_Vec2 pos, mu_Color color) {
    f32 x_advance = 0;
    for (const char *char_ptr = text; *char_ptr; char_ptr++) {
        // Skip continuation bytes
        if ((*char_ptr & 0xc0) == 0x80) {
            continue;
        }

        int32_t codepoint = (int32_t)*char_ptr - 32;
        if (codepoint < 0 || codepoint >= 96) {
            continue;
        }

        stbtt_bakedchar char_data = font_atlas->char_data[codepoint];

        FloatRect src = {.x = (f32)char_data.x0,
                         .y = (f32)char_data.y0,
                         .w = (f32)char_data.x1 - char_data.x0,
                         .h = (f32)char_data.y1 - char_data.y0};

        FloatRect dst = {
            .x = (f32)pos.x + char_data.xoff + x_advance,
            .y = (f32)pos.y + char_data.yoff + FONT_MAX_ASCENT,
            .w = (f32)src.w,
            .h = (f32)src.h,
        };

        PushUIQuad(font_atlas, data, dst, src, color);
        x_advance += char_data.xadvance;
    }
}


void FlushUI(Graphics *gfx, Renderer *renderer, VkCommandBuffer command_buffer,
             UIVertexData *ui_vertex_data, FrameData *frame_data) {
    Vkd vkd = gfx->vkd;

    UIVert *gpu_verts;
    uint16_t *gpu_indices;
    vkd.vkMapMemory(gfx->device,
                    frame_data->vertex_buffer.memory,
                    0, frame_data->vertex_buffer.size, 0, (void *)&gpu_verts);
    vkd.vkMapMemory(gfx->device,
                    frame_data->index_buffer.memory,
                    0, frame_data->index_buffer.size, 0, (void *)&gpu_indices);

    memcpy(gpu_verts, ui_vertex_data->verts, ui_vertex_data->vert_count * sizeof(UIVert));
    memcpy(gpu_indices, ui_vertex_data->indices, ui_vertex_data->index_count * sizeof(uint16_t));

    vkd.vkUnmapMemory(gfx->device, frame_data->vertex_buffer.memory);
    vkd.vkUnmapMemory(gfx->device, frame_data->index_buffer.memory);

    VkDeviceSize offset = 0;
    vkd.vkCmdBindVertexBuffers(command_buffer, 0, 1, &frame_data->vertex_buffer.handle, &offset);
    vkd.vkCmdBindIndexBuffer(command_buffer, frame_data->index_buffer.handle, 0, VK_INDEX_TYPE_UINT16);

    vkd.vkCmdDrawIndexed(command_buffer, ui_vertex_data->index_count, 1, 0, 0, 0);

    ui_vertex_data->vert_count = 0;
    ui_vertex_data->index_count = 0;
}

Frame GetNextFrame(Graphics* gfx, Renderer *renderer, uint64_t frame_count) {
    renderer->frame_index = frame_count % FRAME_COUNT;
    Frame this_frame = renderer->frames[renderer->frame_index];
    this_frame.image = gfx->swapchain_images[renderer->frame_index];
    this_frame.image_view = gfx->swapchain_image_views[renderer->frame_index];
    return this_frame;
}

void RendererDrawUIFrame(Arena *scratch_arena, Renderer *renderer, Graphics *gfx, UI *ui, FontAtlas *font_atlas, uint32_t frame_count) {
    Vkd vkd = gfx->vkd;

    Frame frame = GetNextFrame(gfx, renderer, frame_count);
    Pipeline *pipeline = &gfx->ui_pipeline;

    // Wait for fence
    uint64_t max_wait_ms = 0;
    VkResult fence_response = vkd.vkWaitForFences(gfx->device, 1, &frame.in_flight_fence, VK_TRUE, max_wait_ms);
    if (fence_response != VK_SUCCESS) {
        SDL_Log("Fence timeout\n");
        return;
    }
    VkResult reset_response = vkd.vkResetFences(gfx->device, 1, &frame.in_flight_fence);
    if (reset_response != VK_SUCCESS) {
        SDL_Log("Failed to reset fence\n");
        return;
    }

    // Begin recording
    VkCommandBufferBeginInfo begin_info = {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    vkd.vkBeginCommandBuffer(frame.command_buffer, &begin_info);
    TransitionImageForRendering(gfx, frame.command_buffer, frame.image);

    VkClearValue clear_val = {
        .color = (VkClearColorValue){
            .float32 = {0.0f, 0.0f, 0.0f, 1.0f}}};

    VkRenderingAttachmentInfo color_attachment_info = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = frame.image_view,
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = clear_val};

    VkRenderingInfo rendering_info = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = {
            .offset = {0, 0},
            .extent = gfx->extent},
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &color_attachment_info};

    vkd.vkCmdBeginRendering(frame.command_buffer, &rendering_info);

    VkViewport viewport = {
        .x = 0.0f, .y = 0.0f, .width = (f32)gfx->extent.width, .height = (f32)gfx->extent.height, .minDepth = 0.0f, .maxDepth = 1.0f};
    vkd.vkCmdSetViewport(frame.command_buffer, 0, 1, &viewport);

    vkd.vkCmdBindPipeline(frame.command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->handle);

    VkDescriptorSet descriptor_sets[] = {
        renderer->ui_projection_sets[renderer->frame_index],
        renderer->ui_texture_sets[renderer->frame_index]};

    vkd.vkCmdBindDescriptorSets(frame.command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->layout, 0, 2,
                                descriptor_sets, 0, NULL);

    // UI space projection matrix
    {
        HMM_Mat4 projection = {
            .Elements = {
                {2.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, -2.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {-1.0f, 1.0f, 0.0f, 1.0f}},
        };
        projection.Elements[0][0] /= gfx->extent.width;
        projection.Elements[1][1] /= gfx->extent.height;

        void *data;
        vkd.vkMapMemory(gfx->device, frame.ui_frame_data.uniform_buffer.memory, 0, sizeof(projection), 0, &data);
        memcpy(data, &projection, sizeof(projection));
        vkd.vkUnmapMemory(gfx->device, frame.ui_frame_data.uniform_buffer.memory);

        VkDescriptorBufferInfo buffer_info = {
            .buffer = frame.ui_frame_data.uniform_buffer.handle,
            .offset = 0,
            .range = sizeof(projection)};
        VkWriteDescriptorSet descriptor_write = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = renderer->ui_projection_sets[renderer->frame_index],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .pBufferInfo = &buffer_info};

        vkd.vkUpdateDescriptorSets(gfx->device, 1, &descriptor_write, 0, NULL);
    }

    // Set initial scissor
    VkRect2D scissor = {
        .offset = {.x = 0, .y = 0},
        .extent = {
            .width = gfx->extent.width,
            .height = gfx->extent.height},
    };
    vkd.vkCmdSetScissor(frame.command_buffer, 0, 1, &scissor);

    // Draw
    {
        UIVertexData *data = ArenaPushStruct(scratch_arena, UIVertexData);
        mu_Command *cmd = NULL;

        while (mu_next_command(&ui->ctx, &cmd)) {
            switch (cmd->type) {
                case MU_COMMAND_TEXT: {
                    DrawUIText(font_atlas, data, cmd->text.str, cmd->text.pos, cmd->text.color);
                } break;
                case MU_COMMAND_ICON: {
                } break;
                case MU_COMMAND_RECT: {
                    FloatRect white_rect = {0, 0, 0, 0};
                    FloatRect dest_rect = {.x = (f32)cmd->rect.rect.x,
                                           .y = (f32)cmd->rect.rect.y,
                                           .w = (f32)cmd->rect.rect.w,
                                           .h = (f32)cmd->rect.rect.h};
                    PushUIQuad(font_atlas, data,
                               dest_rect, white_rect,
                               cmd->rect.color);
                } break;
                case MU_COMMAND_CLIP: {
                    int32_t scissor_y = gfx->extent.height - (cmd->clip.rect.y + cmd->clip.rect.h);
                    scissor_y = max(scissor_y, 0);
                    VkRect2D scissor = {
                        .offset = {.x = cmd->clip.rect.x, .y = scissor_y},
                        .extent = {
                            .width = cmd->clip.rect.w,
                            .height = cmd->clip.rect.h,
                        },
                    };
                    vkd.vkCmdSetScissor(frame.command_buffer, 0, 1, &scissor);
                    FlushUI(gfx, renderer, frame.command_buffer, data, &frame.ui_frame_data);
                } break;
            }
        }

        FlushUI(gfx, renderer, frame.command_buffer, data, &frame.ui_frame_data);
    }


    vkd.vkCmdEndRendering(frame.command_buffer);
    TransitionImageForPresent(gfx, frame.command_buffer, frame.image);
    VkResult result = vkd.vkEndCommandBuffer(frame.command_buffer);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to record UI command buffer\n");
        return;
    }



    // Submit
    {
        VkSemaphore image_ready_semaphore = frame.image_ready_semaphore;
        VkSemaphore render_finished_semaphore = frame.render_finished_semaphore;

        uint32_t image_index;
        VkResult acquire_response = vkd.vkAcquireNextImageKHR(gfx->device, gfx->swapchain,
                                                              max_wait_ms, image_ready_semaphore,
                                                              VK_NULL_HANDLE, &image_index);

        if (acquire_response != VK_SUCCESS) {
            SDL_Log("Failed to acquire next image\n");
            return;
        }

        VkPipelineStageFlags wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pWaitDstStageMask = &wait_stages,
            .pCommandBuffers = &frame.command_buffer,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &image_ready_semaphore,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &render_finished_semaphore};

        VkResult submit_response = vkd.vkQueueSubmit(renderer->queue, 1, &submit_info, frame.in_flight_fence);
        if (submit_response != VK_SUCCESS) {
            SDL_Log("Failed to submit to queue\n");
            return;
        }

        VkPresentInfoKHR present_info = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &render_finished_semaphore,
            .swapchainCount = 1,
            .pSwapchains = &gfx->swapchain,
            .pImageIndices = &image_index};

        VkResult present_response = vkd.vkQueuePresentKHR(renderer->queue, &present_info);
        if (present_response != VK_SUCCESS) {
            SDL_Log("Failed to present\n");
            return;
        }
    }
}

void LoadCommandBuffers(Graphics *gfx, Renderer *renderer) {
    VkCommandPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .queueFamilyIndex = gfx->gpu.queue_family_index,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    };

    gfx->vkd.vkCreateCommandPool(gfx->device, &pool_info, NULL, &renderer->command_pool);

    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool = renderer->command_pool,
        .commandBufferCount = FRAME_COUNT};

    VkCommandBuffer command_buffers[FRAME_COUNT];
    gfx->vkd.vkAllocateCommandBuffers(gfx->device, &alloc_info, command_buffers);

    for (uint32_t i = 0; i < FRAME_COUNT; i++) {
        renderer->frames[i].command_buffer = command_buffers[i];
    }
}

VkSemaphore GetSemaphore(Graphics *gfx) {
    VkSemaphoreCreateInfo semaphore_info = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    VkSemaphore semaphore;
    gfx->vkd.vkCreateSemaphore(gfx->device, &semaphore_info, NULL, &semaphore);
    return semaphore;
}

bool DestroyFrameData(Graphics *gfx, FrameData *frame_data) {
    Vkd vkd = gfx->vkd;
    vkd.vkDestroyBuffer(gfx->device, frame_data->vertex_buffer.handle, NULL);
    vkd.vkFreeMemory(gfx->device, frame_data->vertex_buffer.memory, NULL);
    vkd.vkDestroyBuffer(gfx->device, frame_data->index_buffer.handle, NULL);
    vkd.vkFreeMemory(gfx->device, frame_data->index_buffer.memory, NULL);
    vkd.vkDestroyBuffer(gfx->device, frame_data->uniform_buffer.handle, NULL);
    vkd.vkFreeMemory(gfx->device, frame_data->uniform_buffer.memory, NULL);
    return true;
}

void LoadFrameData(Graphics *gfx, FrameData *out_frame_data) {
    VkDeviceSize ui_vertex_buffer_size = GetAlignedSize(gfx, sizeof(UIVert) * MAX_UI_VERTS);
    out_frame_data->vertex_buffer.size = ui_vertex_buffer_size;
    bool result = GetBuffer(gfx, ui_vertex_buffer_size,
                            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                            &out_frame_data->vertex_buffer);

    if (!result) {
        ExitWithError("Failed to get UI vertex buffer");
    }
    VkDeviceSize ui_index_buffer_size = GetAlignedSize(gfx, sizeof(uint16_t) * MAX_UI_INDICES);
    out_frame_data->index_buffer.size = ui_index_buffer_size;
    result = GetBuffer(gfx, ui_index_buffer_size,
                       VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       &out_frame_data->index_buffer);
    if (!result) {
        ExitWithError("Failed to get UI index buffer");
    }

    VkDeviceSize ui_uniform_buffer_size = GetAlignedSize(gfx, sizeof(HMM_Mat4));
    out_frame_data->uniform_buffer.size = ui_uniform_buffer_size;
    result = GetBuffer(gfx, ui_uniform_buffer_size,
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       &out_frame_data->uniform_buffer);

    if (!result) {
        ExitWithError("Failed to get UI uniform buffer");
    }
};

void LoadPerFrameObjects(Graphics *gfx, Renderer *renderer) {
    for (uint32_t i = 0; i < FRAME_COUNT; i++) {
        LoadFrameData(gfx, &renderer->frames[i].ui_frame_data);

        SetUITexture(gfx, renderer, &renderer->ui_texture_sets[i]);

        VkFenceCreateInfo fence_info = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT};
        gfx->vkd.vkCreateFence(gfx->device, &fence_info, NULL, &renderer->frames[i].in_flight_fence);

        VkSemaphoreCreateInfo semaphore_info = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        gfx->vkd.vkCreateSemaphore(gfx->device, &semaphore_info, NULL, &renderer->frames[i].image_ready_semaphore);
        gfx->vkd.vkCreateSemaphore(gfx->device, &semaphore_info, NULL, &renderer->frames[i].render_finished_semaphore);
    }
}

void RendererDestroy(Renderer *renderer, Graphics *gfx) {
    Vkd vkd = gfx->vkd;

    vkd.vkDeviceWaitIdle(gfx->device);

    for (uint32_t i = 0; i < FRAME_COUNT; i++) {
        vkd.vkDestroyFence(gfx->device, renderer->frames[i].in_flight_fence, NULL);
        vkd.vkDestroySemaphore(gfx->device, renderer->frames[i].image_ready_semaphore, NULL);
        vkd.vkDestroySemaphore(gfx->device, renderer->frames[i].render_finished_semaphore, NULL);

        DestroyFrameData(gfx, &renderer->frames[i].ui_frame_data);
    }

    vkd.vkDestroySampler(gfx->device, renderer->font_texture.sampler, NULL);
    vkd.vkDestroyImageView(gfx->device, renderer->font_texture.image_view, NULL);
    vkd.vkDestroyImage(gfx->device, renderer->font_texture.image, NULL);
    vkd.vkFreeMemory(gfx->device, renderer->font_texture.image_memory, NULL);

    vkd.vkFreeDescriptorSets(gfx->device, renderer->descriptor_pool, FRAME_COUNT, renderer->ui_projection_sets);
    vkd.vkFreeDescriptorSets(gfx->device, renderer->descriptor_pool, FRAME_COUNT, renderer->ui_texture_sets);

    vkd.vkDestroyDescriptorPool(gfx->device, renderer->descriptor_pool, NULL);
    vkd.vkDestroyCommandPool(gfx->device, renderer->command_pool, NULL);
}

Renderer *RendererInit(Arena *arena, Graphics *gfx, FontAtlas *font_atlas) {
    Renderer *renderer = ArenaPushStruct(arena, Renderer);

    LoadCommandBuffers(gfx, renderer);
    vkGetDeviceQueue(gfx->device, gfx->gpu.queue_family_index, 0, &renderer->queue);
    LoadFontTexture(gfx, renderer, font_atlas);

    renderer->descriptor_pool = GetDescriptorPool(gfx);
    renderer->ui_projection_sets = GetUIDescriptorSets(arena, gfx,
                                                            renderer->descriptor_pool,
                                                            gfx->ui_pipeline.projection_set_layout);
    renderer->ui_texture_sets = GetUIDescriptorSets(arena, gfx,
                                                            renderer->descriptor_pool,
                                                            gfx->ui_pipeline.texture_set_layout);

    LoadPerFrameObjects(gfx, renderer);

    return renderer;
}
