#include "graphics.h"

void LoadVulkanDispatchTable(Graphics* gfx) {
    VkDevice device = gfx->device;
    Vkd vkd = {0};

    vkd.vkCreateImageView = (PFN_vkCreateImageView)vkGetDeviceProcAddr(device, "vkCreateImageView");
    vkd.vkDestroyImageView = (PFN_vkDestroyImageView)vkGetDeviceProcAddr(device, "vkDestroyImageView");
    vkd.vkCreateImage = (PFN_vkCreateImage)vkGetDeviceProcAddr(device, "vkCreateImage");
    vkd.vkDestroyImage = (PFN_vkDestroyImage)vkGetDeviceProcAddr(device, "vkDestroyImage");

    vkd.vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)vkGetDeviceProcAddr(device, "vkGetImageMemoryRequirements");
    vkd.vkBindImageMemory = (PFN_vkBindImageMemory)vkGetDeviceProcAddr(device, "vkBindImageMemory");
    vkd.vkCreateSampler = (PFN_vkCreateSampler)vkGetDeviceProcAddr(device, "vkCreateSampler");
    vkd.vkDestroySampler = (PFN_vkDestroySampler)vkGetDeviceProcAddr(device, "vkDestroySampler");

    vkd.vkCreateShaderModule = (PFN_vkCreateShaderModule)vkGetDeviceProcAddr(device, "vkCreateShaderModule");
    vkd.vkDestroyShaderModule = (PFN_vkDestroyShaderModule)vkGetDeviceProcAddr(device, "vkDestroyShaderModule");

    vkd.vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout)vkGetDeviceProcAddr(device, "vkCreatePipelineLayout");
    vkd.vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)vkGetDeviceProcAddr(device, "vkDestroyPipelineLayout");
    vkd.vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)vkGetDeviceProcAddr(device, "vkCreateGraphicsPipelines");
    vkd.vkDestroyPipeline = (PFN_vkDestroyPipeline)vkGetDeviceProcAddr(device, "vkDestroyPipeline");

    vkd.vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)vkGetDeviceProcAddr(device, "vkCreateDescriptorSetLayout");
    vkd.vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)vkGetDeviceProcAddr(device, "vkDestroyDescriptorSetLayout");

    vkd.vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool)vkGetDeviceProcAddr(device, "vkCreateDescriptorPool");
    vkd.vkDestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)vkGetDeviceProcAddr(device, "vkDestroyDescriptorPool");

    vkd.vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)vkGetDeviceProcAddr(device, "vkAllocateDescriptorSets");
    vkd.vkFreeDescriptorSets = (PFN_vkFreeDescriptorSets)vkGetDeviceProcAddr(device, "vkFreeDescriptorSets");
    vkd.vkUpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)vkGetDeviceProcAddr(device, "vkUpdateDescriptorSets");

    vkd.vkCreateBuffer = (PFN_vkCreateBuffer)vkGetDeviceProcAddr(device, "vkCreateBuffer");
    vkd.vkDestroyBuffer = (PFN_vkDestroyBuffer)vkGetDeviceProcAddr(device, "vkDestroyBuffer");
    vkd.vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)vkGetDeviceProcAddr(device, "vkGetBufferMemoryRequirements");
    vkd.vkAllocateMemory = (PFN_vkAllocateMemory)vkGetDeviceProcAddr(device, "vkAllocateMemory");
    vkd.vkBindBufferMemory = (PFN_vkBindBufferMemory)vkGetDeviceProcAddr(device, "vkBindBufferMemory");
    vkd.vkFreeMemory = (PFN_vkFreeMemory)vkGetDeviceProcAddr(device, "vkFreeMemory");
    vkd.vkMapMemory = (PFN_vkMapMemory)vkGetDeviceProcAddr(device, "vkMapMemory");
    vkd.vkUnmapMemory = (PFN_vkUnmapMemory)vkGetDeviceProcAddr(device, "vkUnmapMemory");

    vkd.vkCreateFramebuffer = (PFN_vkCreateFramebuffer)vkGetDeviceProcAddr(device, "vkCreateFramebuffer");
    vkd.vkDestroyFramebuffer = (PFN_vkDestroyFramebuffer)vkGetDeviceProcAddr(device, "vkDestroyFramebuffer");

    vkd.vkCreateCommandPool = (PFN_vkCreateCommandPool)vkGetDeviceProcAddr(device, "vkCreateCommandPool");
    vkd.vkDestroyCommandPool = (PFN_vkDestroyCommandPool)vkGetDeviceProcAddr(device, "vkDestroyCommandPool");

    vkd.vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)vkGetDeviceProcAddr(device, "vkAllocateCommandBuffers");
    vkd.vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers)vkGetDeviceProcAddr(device, "vkFreeCommandBuffers");
    vkd.vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)vkGetDeviceProcAddr(device, "vkBeginCommandBuffer");
    vkd.vkEndCommandBuffer = (PFN_vkEndCommandBuffer)vkGetDeviceProcAddr(device, "vkEndCommandBuffer");

    vkd.vkCmdBindPipeline = (PFN_vkCmdBindPipeline)vkGetDeviceProcAddr(device, "vkCmdBindPipeline");
    vkd.vkCmdDraw = (PFN_vkCmdDraw)vkGetDeviceProcAddr(device, "vkCmdDraw");
    vkd.vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)vkGetDeviceProcAddr(device, "vkCmdDrawIndexed");
    vkd.vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)vkGetDeviceProcAddr(device, "vkCmdPipelineBarrier");
    vkd.vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)vkGetDeviceProcAddr(device, "vkCmdBindVertexBuffers");
    vkd.vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)vkGetDeviceProcAddr(device, "vkCmdBindIndexBuffer");
    vkd.vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)vkGetDeviceProcAddr(device, "vkCmdBindDescriptorSets");
    vkd.vkCmdSetScissor = (PFN_vkCmdSetScissor)vkGetDeviceProcAddr(device, "vkCmdSetScissor");
    vkd.vkCmdSetViewport = (PFN_vkCmdSetViewport)vkGetDeviceProcAddr(device, "vkCmdSetViewport");
    vkd.vkCmdCopyBuffer = (PFN_vkCmdCopyBuffer)vkGetDeviceProcAddr(device, "vkCmdCopyBuffer");
    vkd.vkCmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage)vkGetDeviceProcAddr(device, "vkCmdCopyBufferToImage");

    vkd.vkCreateSemaphore = (PFN_vkCreateSemaphore)vkGetDeviceProcAddr(device, "vkCreateSemaphore");
    vkd.vkDestroySemaphore = (PFN_vkDestroySemaphore)vkGetDeviceProcAddr(device, "vkDestroySemaphore");
    vkd.vkCreateFence = (PFN_vkCreateFence)vkGetDeviceProcAddr(device, "vkCreateFence");
    vkd.vkDestroyFence = (PFN_vkDestroyFence)vkGetDeviceProcAddr(device, "vkDestroyFence");
    vkd.vkWaitForFences = (PFN_vkWaitForFences)vkGetDeviceProcAddr(device, "vkWaitForFences");
    vkd.vkResetFences = (PFN_vkResetFences)vkGetDeviceProcAddr(device, "vkResetFences");

    vkd.vkQueueSubmit = (PFN_vkQueueSubmit)vkGetDeviceProcAddr(device, "vkQueueSubmit");
    vkd.vkQueueWaitIdle = (PFN_vkQueueWaitIdle)vkGetDeviceProcAddr(device, "vkQueueWaitIdle");
    vkd.vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle)vkGetDeviceProcAddr(device, "vkDeviceWaitIdle");

    // Extensions
    vkd.vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)vkGetDeviceProcAddr(device, "vkCreateSwapchainKHR");
    vkd.vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)vkGetDeviceProcAddr(device, "vkDestroySwapchainKHR");
    vkd.vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)vkGetDeviceProcAddr(device, "vkGetSwapchainImagesKHR");
    vkd.vkQueuePresentKHR = (PFN_vkQueuePresentKHR)vkGetDeviceProcAddr(device, "vkQueuePresentKHR");
    vkd.vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)vkGetDeviceProcAddr(device, "vkAcquireNextImageKHR");

    vkd.vkCmdBeginRendering = (PFN_vkCmdBeginRendering)vkGetDeviceProcAddr(device, "vkCmdBeginRendering");
    vkd.vkCmdEndRendering = (PFN_vkCmdEndRendering)vkGetDeviceProcAddr(device, "vkCmdEndRendering");

    gfx->vkd = vkd;
}

VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                       VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                       const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                       void* pUserData) {
    SDL_Log("vk: %s", pCallbackData->pMessage);
    return VK_FALSE;
}

void LoadDebugMessenger(Graphics* gfx) {
    PFN_vkCreateDebugUtilsMessengerEXT debug_fn = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(gfx->instance, "vkCreateDebugUtilsMessengerEXT");
    if (debug_fn == NULL) {
        SDL_Log("vkCreateDebugUtilsMessengerEXT not found");
        ExitWithError("vkCreateDebugUtilsMessengerEXT not found");
    }

    VkDebugUtilsMessageSeverityFlagsEXT severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    VkDebugUtilsMessageTypeFlagsEXT type = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = severity,
        .messageType = type,
        .pfnUserCallback = DebugCallback};

    VkDebugUtilsMessengerEXT debug_messenger;
    debug_fn(gfx->instance, &debug_messenger_info, NULL, &debug_messenger);

    gfx->debug_messenger = debug_messenger;
}

void PipelineDestroy(Pipeline* pipeline, Graphics* gfx) {
    if (pipeline->projection_set_layout != NULL) {
        gfx->vkd.vkDestroyDescriptorSetLayout(gfx->device, pipeline->projection_set_layout, NULL);
    }
    if (pipeline->texture_set_layout != NULL) {
        gfx->vkd.vkDestroyDescriptorSetLayout(gfx->device, pipeline->texture_set_layout, NULL);
    }

    gfx->vkd.vkDestroyPipelineLayout(gfx->device, pipeline->layout, NULL);
    gfx->vkd.vkDestroyPipeline(gfx->device, pipeline->handle, NULL);
}

VkPipelineShaderStageCreateInfo GetShader(Arena* arena, Graphics* gfx, char* filename, VkShaderStageFlagBits stage) {
    FileBuffer code = LoadFile(arena, filename);
    VkShaderModuleCreateInfo module_info = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code.size,
        .pCode = (const uint32_t*)code.base,
    };

    VkShaderModule module;
    VkResult result = gfx->vkd.vkCreateShaderModule(gfx->device, &module_info, NULL, &module);
    assert(result == VK_SUCCESS);

    VkPipelineShaderStageCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = stage,
        .module = module,
        .pName = "main",
    };
    return create_info;
}

VkDescriptorSetLayout GetProjectionSetLayout(Graphics* gfx) {
    VkDescriptorSetLayoutBinding ubo_layout_binding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = NULL,
    };

    VkDescriptorSetLayoutCreateInfo ubo_layout_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &ubo_layout_binding,
    };

    VkDescriptorSetLayout ubo_layout;
    VkResult result = gfx->vkd.vkCreateDescriptorSetLayout(gfx->device, &ubo_layout_info, NULL, &ubo_layout);
    assert(result == VK_SUCCESS);

    return ubo_layout;
}

VkDescriptorSetLayout GetTextureSetLayout(Graphics* gfx) {
    VkDescriptorSetLayoutBinding sampler_layout_binding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = NULL,
    };
    VkDescriptorSetLayoutCreateInfo sampler_layout_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &sampler_layout_binding,
    };

    VkDescriptorSetLayout sampler_layout;
    VkResult result = gfx->vkd.vkCreateDescriptorSetLayout(gfx->device, &sampler_layout_info, NULL, &sampler_layout);
    assert(result == VK_SUCCESS);

    return sampler_layout;
}

void LoadUIPipeline(Arena* arena, Graphics* gfx) {
    Arena* temp_arena = ArenaInit(Megabytes(8));

    VkPipelineShaderStageCreateInfo shader_stages[2] = {
        GetShader(temp_arena, gfx, "ui_vert.spv", VK_SHADER_STAGE_VERTEX_BIT),
        GetShader(temp_arena, gfx, "ui_frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT),
    };

    VkVertexInputBindingDescription binding_description[1] = {
        {
            .binding = 0,
            .stride = sizeof(UIVert),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        }};

    VkVertexInputAttributeDescription attribute_descriptions[3] = {
        {
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = offsetof(UIVert, pos),
        },
        {
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = offsetof(UIVert, uv),
        },
        {
            .location = 2,
            .binding = 0,
            .format = VK_FORMAT_R8G8B8A8_UINT,
            .offset = offsetof(UIVert, colour),
        },
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = binding_description,
        .vertexAttributeDescriptionCount = 3,
        .pVertexAttributeDescriptions = attribute_descriptions,
    };

    VkPipelineInputAssemblyStateCreateInfo input_assembly_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    VkPipelineRasterizationStateCreateInfo rasterization_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_NONE,
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .lineWidth = 1.0f,
    };

    VkPipelineMultisampleStateCreateInfo multisample_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    };

    VkPipelineColorBlendAttachmentState color_blend_attachment = {
        .blendEnable = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };

    VkPipelineColorBlendStateCreateInfo color_blend_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &color_blend_attachment,
        .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f},
    };

    VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamic_state_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates = dynamic_states,
    };

    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (f32)gfx->extent.width,
        .height = (f32)gfx->extent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    VkRect2D scissor = {
        .offset = {0, 0},
        .extent = gfx->extent,
    };

    VkPipelineViewportStateCreateInfo viewport_state_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor,
    };

    Pipeline* pipeline = &gfx->ui_pipeline;

    pipeline->projection_set_layout = GetProjectionSetLayout(gfx);
    pipeline->texture_set_layout = GetTextureSetLayout(gfx);

    VkDescriptorSetLayout set_layouts[2] = {
        pipeline->projection_set_layout,
        pipeline->texture_set_layout,
    };

    VkPipelineRenderingCreateInfoKHR rendering_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
        .pNext = NULL,
        .colorAttachmentCount = 1,
        .pColorAttachmentFormats = &gfx->surface_format.format,
    };

    VkPipelineLayoutCreateInfo pipeline_layout_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 2,
        .pSetLayouts = set_layouts,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = NULL,
    };

    gfx->vkd.vkCreatePipelineLayout(gfx->device, &pipeline_layout_info, NULL, &pipeline->layout);

    VkGraphicsPipelineCreateInfo pipeline_create_info = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = shader_stages,
        .pVertexInputState = &vertex_input_info,
        .pInputAssemblyState = &input_assembly_info,
        .pRasterizationState = &rasterization_info,
        .pMultisampleState = &multisample_info,
        .pColorBlendState = &color_blend_info,
        .pDynamicState = &dynamic_state_info,
        .pViewportState = &viewport_state_info,
        .layout = pipeline->layout,
        .basePipelineIndex = -1,
        .basePipelineHandle = VK_NULL_HANDLE,
        .subpass = 0,
        .renderPass = VK_NULL_HANDLE,
        .pNext = &rendering_info,
    };

    VkResult result = gfx->vkd.vkCreateGraphicsPipelines(gfx->device, VK_NULL_HANDLE, 1, &pipeline_create_info, NULL, &pipeline->handle);

    gfx->vkd.vkDestroyShaderModule(gfx->device, shader_stages[0].module, NULL);
    gfx->vkd.vkDestroyShaderModule(gfx->device, shader_stages[1].module, NULL);
    ArenaDestroy(temp_arena);

    if (result != VK_SUCCESS) {
        ExitWithError("Failed to create UI pipeline");
    }
}

void LoadSwapchainImageViews(Arena* arena, Graphics* gfx) {
    VkImageView* image_views = ArenaPushArray(arena, VkImageView, gfx->swapchain_image_count);

    for (uint32_t i = 0; i < gfx->swapchain_image_count; i++) {
        VkImageViewCreateInfo image_view_create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = gfx->swapchain_images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = gfx->surface_format.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
        };

        VkResult result = gfx->vkd.vkCreateImageView(gfx->device, &image_view_create_info, NULL, &image_views[i]);
        if (result != VK_SUCCESS) {
            ExitWithError("Failed to create image view");
        }
    }

    gfx->swapchain_image_views = image_views;
}

void LoadSwapchainImages(Arena* arena, Graphics* gfx) {
    uint32_t image_count;
    gfx->vkd.vkGetSwapchainImagesKHR(gfx->device, gfx->swapchain, &image_count, NULL);

    VkImage* images = ArenaPushArray(arena, VkImage, image_count);
    gfx->vkd.vkGetSwapchainImagesKHR(gfx->device, gfx->swapchain, &image_count, images);

    gfx->swapchain_image_count = image_count;
    gfx->swapchain_images = images;
}

void LoadSwapchain(Graphics* gfx) {
    VkSwapchainCreateInfoKHR swapchain_create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = gfx->surface,
        .minImageCount = FRAME_COUNT,
        .imageFormat = gfx->surface_format.format,
        .imageColorSpace = gfx->surface_format.colorSpace,
        .imageExtent = gfx->extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = gfx->gpu.surface_capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = gfx->present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE,
    };

    VkResult result = gfx->vkd.vkCreateSwapchainKHR(gfx->device, &swapchain_create_info, NULL, &gfx->swapchain);
    if (result != VK_SUCCESS) {
        ExitWithError("Failed to create swapchain");
    }
}

bool CheckPhysicalDeviceExtensions(Arena* arena, VkPhysicalDevice physical_device) {
    uint32_t ext_count;
    vkEnumerateDeviceExtensionProperties(physical_device, NULL, &ext_count, NULL);

    VkExtensionProperties* available_exts = ArenaPushArray(arena, VkExtensionProperties, ext_count);
    vkEnumerateDeviceExtensionProperties(physical_device, NULL, &ext_count, available_exts);

    uint32_t required_ext_count = 0;
    for (uint32_t i = 0; i < ext_count; i++) {
        for (uint32_t j = 0; j < DEVICE_EXTENSIONS_COUNT; j++) {
            if (strcmp(available_exts[i].extensionName, DEVICE_EXTENSIONS[j]) == 0) {
                required_ext_count++;
                break;
            }
        }

        if (required_ext_count == DEVICE_EXTENSIONS_COUNT) {
            return true;
        }
    }

    return false;
}

bool CheckPhysicalDevicePresentModes(Arena* arena,
                                     VkSurfaceKHR surface,
                                     VkPresentModeKHR present_mode,
                                     VkPhysicalDevice physical_device) {
    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, NULL);

    VkPresentModeKHR* present_modes = ArenaPushArray(arena, VkPresentModeKHR, present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes);

    for (uint32_t i = 0; i < present_mode_count; i++) {
        if (present_modes[i] == present_mode) {
            return true;
        }
    }
    return false;
}

bool CheckPhysicalDeviceSurfaceFormats(Arena* arena,
                                       VkSurfaceKHR surface,
                                       VkSurfaceFormatKHR surface_format,
                                       VkPhysicalDevice physical_device) {
    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, NULL);

    VkSurfaceFormatKHR* formats = ArenaPushArray(arena, VkSurfaceFormatKHR, format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats);

    for (uint32_t i = 0; i < format_count; i++) {
        bool is_format_valid = formats[i].format == surface_format.format && formats[i].colorSpace == surface_format.colorSpace;
        if (is_format_valid) {
            return true;
        }
    }
    return false;
}

bool PickGPU(
    Arena* arena,
    Graphics* gfx,
    VkPhysicalDevice physical_device) {
    bool has_swapchain = CheckPhysicalDeviceExtensions(arena, physical_device);
    bool has_present_modes = CheckPhysicalDevicePresentModes(arena, gfx->surface, gfx->present_mode, physical_device);
    bool has_surface_formats = CheckPhysicalDeviceSurfaceFormats(arena, gfx->surface, gfx->surface_format, physical_device);

    if (!has_swapchain || !has_present_modes || !has_surface_formats) {
        return false;
    }

    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);

    VkQueueFamilyProperties* queue_families = ArenaPushArray(arena, VkQueueFamilyProperties, queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families);

    GPU* gpu = &gfx->gpu;
    for (uint32_t i = 0; i < queue_family_count; i++) {
        bool has_graphics = queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;
        bool has_physics = queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT;

        VkBool32 has_surface;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, gfx->surface, &has_surface);

        if (has_graphics && has_physics && has_surface) {
            VkPhysicalDeviceExternalMemoryHostPropertiesEXT external_memory_properties = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT,
                .pNext = NULL,
            };
            VkPhysicalDeviceProperties2 device_properties = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
                .pNext = &external_memory_properties,
            };
            vkGetPhysicalDeviceProperties2(physical_device, &device_properties);
            gpu->min_device_pointer_alignment = external_memory_properties.minImportedHostPointerAlignment;
            gpu->device_properties = device_properties.properties;

            vkGetPhysicalDeviceMemoryProperties(physical_device, &gpu->memory_properties);

            VkResult caps_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, gfx->surface, &gpu->surface_capabilities);
            if (caps_result != VK_SUCCESS) {
                return false;
            }

            gpu->queue_family_index = i;
            gpu->physical_device = physical_device;
            return true;
        }
    }

    return false;
}

void LoadPhysicalDevice(Arena* arena, Graphics* gfx) {
    gfx->surface_format = (VkSurfaceFormatKHR){
        .format = VK_FORMAT_B8G8R8A8_UNORM,
        .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    gfx->present_mode = VK_PRESENT_MODE_FIFO_KHR;

    uint32_t physical_device_count;
    vkEnumeratePhysicalDevices(gfx->instance, &physical_device_count, NULL);
    VkPhysicalDevice* physical_devices = ArenaPushArray(arena, VkPhysicalDevice, physical_device_count);
    vkEnumeratePhysicalDevices(gfx->instance, &physical_device_count, physical_devices);

    for (uint32_t i = 0; i < physical_device_count; i++) {
        bool has_valid_gpu = PickGPU(arena, gfx, physical_devices[i]);
        if (has_valid_gpu) {
            return;
        }
    }

    ExitWithError("Failed to find suitable physical device");
}

void LoadLogicalDevice(Graphics* gfx) {
    f32 queue_priorities[1] = {1.0f};
    VkDeviceQueueCreateInfo queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = gfx->gpu.queue_family_index,
        .queueCount = 1,
        .pQueuePriorities = queue_priorities};

    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_feature = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
        .dynamicRendering = VK_TRUE,
    };

    VkDeviceCreateInfo device_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue_info,
        .enabledExtensionCount = DEVICE_EXTENSIONS_COUNT,
        .ppEnabledExtensionNames = DEVICE_EXTENSIONS,
        .pNext = &dynamic_rendering_feature,
        .enabledLayerCount = 0
    };

    VkResult result = vkCreateDevice(gfx->gpu.physical_device, &device_info, NULL, &gfx->device);
    if (result != VK_SUCCESS) {
        ExitWithError("Failed to create logical device");
    }
}

void LoadInstance(Platform* platform, Graphics* gfx) {
    Arena* temp_arena = ArenaInit(1024);

    uint32_t sdl_ext_count = 0;
    if (!SDL_Vulkan_GetInstanceExtensions(platform->window, &sdl_ext_count, NULL)) {
        LogSDLError();
        Exit();
    }
    const char** sdl_instance_ext_names = ArenaPushArray(temp_arena, const char*, sdl_ext_count);
    SDL_Vulkan_GetInstanceExtensions(platform->window, &sdl_ext_count, sdl_instance_ext_names);

    uint32_t instance_ext_count = sdl_ext_count + 1;
    char** instance_ext_names = ArenaPushArray(temp_arena, char*, instance_ext_count);
    for (uint32_t i = 0; i < instance_ext_count; i++) {
        SDL_Log("Adding extension: %s\n", sdl_instance_ext_names[i]);
        if (i == instance_ext_count - 1) {
            instance_ext_names[i] = ArenaPushString(temp_arena, "VK_EXT_debug_utils");
        } else {
            instance_ext_names[i] = ArenaPushString(temp_arena, sdl_instance_ext_names[i]);
        }
    }

    const char* validation_layers[] = {"VK_LAYER_KHRONOS_validation"};

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Microui Sample App",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "Trjan Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3};

    VkInstanceCreateInfo instance_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledExtensionCount = instance_ext_count,
        .ppEnabledExtensionNames = instance_ext_names,
        .enabledLayerCount = 1,
        .ppEnabledLayerNames = validation_layers};

    VkResult result = vkCreateInstance(&instance_info, NULL, &gfx->instance);

    ArenaDestroy(temp_arena);

    if (result != VK_SUCCESS) {
        ExitWithError("Failed to create instance");
    }
}

void CheckSupportedImageCount(Graphics* gfx) {
    uint32_t max_image_count = gfx->gpu.surface_capabilities.maxImageCount;
    uint32_t min_image_count = gfx->gpu.surface_capabilities.minImageCount;
    if (FRAME_COUNT < min_image_count || FRAME_COUNT > max_image_count) {
        ExitWithError("Unsupported image count");
    }
}

Graphics* GraphicsInit(Arena* arena, Platform* platform, VkExtent2D extent) {
    Graphics* gfx = ArenaPushStruct(arena, Graphics);
    gfx->extent = extent;

    LoadInstance(platform, gfx);
    LoadDebugMessenger(gfx);

    bool surface_result = SDL_Vulkan_CreateSurface(platform->window, gfx->instance, &gfx->surface);
    if (surface_result == SDL_FALSE) {
        LogSDLError();
        Exit();
    }

    LoadPhysicalDevice(arena, gfx);
    LoadLogicalDevice(gfx);
    LoadVulkanDispatchTable(gfx);

    CheckSupportedImageCount(gfx);
    LoadSwapchain(gfx);
    LoadSwapchainImages(arena, gfx);
    LoadSwapchainImageViews(arena, gfx);

    LoadUIPipeline(arena, gfx);

    return gfx;
}

void GraphicsDestroy(Graphics* gfx) {
    gfx->vkd.vkDeviceWaitIdle(gfx->device);

    PipelineDestroy(&gfx->ui_pipeline, gfx);

    for (uint32_t i = 0; i < gfx->swapchain_image_count; i++) {
        vkDestroyImageView(gfx->device, gfx->swapchain_image_views[i], NULL);
    }
    gfx->vkd.vkDestroySwapchainKHR(gfx->device, gfx->swapchain, NULL);

    vkDestroyDevice(gfx->device, NULL);
    vkDestroySurfaceKHR(gfx->instance, gfx->surface, NULL);

    PFN_vkDestroyDebugUtilsMessengerEXT debug_destroy_fn = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(gfx->instance, "vkDestroyDebugUtilsMessengerEXT");
    debug_destroy_fn(gfx->instance, gfx->debug_messenger, NULL);

    vkDestroyInstance(gfx->instance, NULL);
}
