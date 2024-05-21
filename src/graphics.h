#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>
#include <hmm/HandmadeMath.h>

#include "arena.h"
#include "platform.h"
#include "defines.h"
#include "ui.h"
#include "file.h"

typedef struct Vkd {
    PFN_vkCreateImage vkCreateImage;
    PFN_vkDestroyImage vkDestroyImage;
    PFN_vkCreateImageView vkCreateImageView;
    PFN_vkDestroyImageView vkDestroyImageView;
    PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
    PFN_vkBindImageMemory vkBindImageMemory;
    PFN_vkCreateSampler vkCreateSampler;
    PFN_vkDestroySampler vkDestroySampler;

    PFN_vkCreateShaderModule vkCreateShaderModule;
    PFN_vkDestroyShaderModule vkDestroyShaderModule;
    PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
    PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
    PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
    PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
    PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
    PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
    PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
    PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
    PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;

    PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
    PFN_vkDestroyPipeline vkDestroyPipeline;
    PFN_vkCreateBuffer vkCreateBuffer;
    PFN_vkDestroyBuffer vkDestroyBuffer;
    PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
    PFN_vkAllocateMemory vkAllocateMemory;
    PFN_vkBindBufferMemory vkBindBufferMemory;
    PFN_vkFreeMemory vkFreeMemory;
    PFN_vkMapMemory vkMapMemory;
    PFN_vkUnmapMemory vkUnmapMemory;

    PFN_vkCreateFramebuffer vkCreateFramebuffer;
    PFN_vkDestroyFramebuffer vkDestroyFramebuffer;

    PFN_vkCreateCommandPool vkCreateCommandPool;
    PFN_vkDestroyCommandPool vkDestroyCommandPool;
    PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
    PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
    PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
    PFN_vkEndCommandBuffer vkEndCommandBuffer;

    PFN_vkCmdBindPipeline vkCmdBindPipeline;
    PFN_vkCmdDraw vkCmdDraw;
    PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
    PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
    PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
    PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
    PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
    PFN_vkCmdSetViewport vkCmdSetViewport;
    PFN_vkCmdSetScissor vkCmdSetScissor;
    PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
    PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;

    PFN_vkCreateSemaphore vkCreateSemaphore;
    PFN_vkDestroySemaphore vkDestroySemaphore;
    PFN_vkCreateFence vkCreateFence;
    PFN_vkDestroyFence vkDestroyFence;
    PFN_vkWaitForFences vkWaitForFences;
    PFN_vkResetFences vkResetFences;

    PFN_vkQueueSubmit vkQueueSubmit;
    PFN_vkQueueWaitIdle vkQueueWaitIdle;
    PFN_vkDeviceWaitIdle vkDeviceWaitIdle;

    // Extensions
    PFN_vkCmdBeginRendering vkCmdBeginRendering;
    PFN_vkCmdEndRendering vkCmdEndRendering;

    PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    PFN_vkQueuePresentKHR vkQueuePresentKHR;
    PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;

} Vkd;

typedef struct GPU {
    VkPhysicalDevice physical_device;
    VkPhysicalDeviceProperties device_properties;
    VkDeviceSize min_device_pointer_alignment;
    uint32_t queue_family_index;
    VkSurfaceCapabilitiesKHR surface_capabilities;
    VkPhysicalDeviceMemoryProperties memory_properties;
} GPU;

typedef struct Pipeline {
    VkPipeline handle;
    VkPipelineLayout layout;
    VkDescriptorSetLayout projection_set_layout;
    VkDescriptorSetLayout texture_set_layout;
} Pipeline;

typedef struct Graphics {
    Vkd vkd;
    VkInstance instance;
    VkDevice device;
    VkDebugUtilsMessengerEXT debug_messenger;

    VkPresentModeKHR present_mode;
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surface_format;
    VkSwapchainKHR swapchain;
    VkExtent2D extent;

    uint32_t swapchain_image_count;
    VkImage* swapchain_images;
    VkImageView* swapchain_image_views;

    GPU gpu;
    Pipeline ui_pipeline;
} Graphics;

#define MAX_UI_VERTS 4096
#define MAX_UI_INDICES 4096

typedef struct UIVert {
    f32 pos[2];
    f32 uv[2];
    uint8_t colour[4];
} UIVert;

const uint32_t DEVICE_EXTENSIONS_COUNT = 3;
const char* const DEVICE_EXTENSIONS[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
    VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
};

Graphics* GraphicsInit(Arena* arena, Platform* platform, VkExtent2D extent);
void GraphicsDestroy(Graphics* gfx);

Pipeline* GetMainPipeline(Arena* arena, Graphics* gfx);
Pipeline* GetUIPipeline(Arena* arena, Graphics* gfx);
void PipelineDestroy(Pipeline* pipeline, Graphics* gfx);
