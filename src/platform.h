#pragma once

#include <SDL2/SDL.h>
#include <SDl2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "arena.h"
#include "defines.h"

typedef struct Platform {
    SDL_Window* window;
    uint32_t width;
    uint32_t height;
} Platform;

Platform* PlatformInit(Arena* arena, VkExtent2D extent);
void PlatformDestroy(Platform* platform);
bool PlatformUpdate(void);
void LogSDLError(void);
void Exit(void);
void ExitWithError(const char* message);

