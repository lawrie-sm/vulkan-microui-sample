#include "platform.h"

void ExitWithError(const char* msg) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", msg);
    exit(1);
}

void Exit(void) {
    exit(1);
}

void LogSDLError(void) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", SDL_GetError());
}

Platform* PlatformInit(Arena* arena, VkExtent2D extent) {
    Platform* platform = ArenaPushStruct(arena, Platform);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        LogSDLError();
        Exit();
    }

    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, extent.width,
                                          extent.height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    if (!window) {
        LogSDLError();
        Exit();
    }
    platform->window = window;

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    platform->width = width;
    platform->height = height;

    return platform;
}

void PlatformDestroy(Platform* platform) {
    SDL_DestroyWindow(platform->window);
    SDL_Quit();
}
