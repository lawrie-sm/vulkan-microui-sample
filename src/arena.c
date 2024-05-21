#include "arena.h"

Arena* ArenaInit(size_t size) {
    Arena* arena = VirtualAlloc(NULL, sizeof(Arena), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (arena == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for arena");
        exit(1);
    }

    void* base = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (base == NULL) {
        VirtualFree(arena, 0, MEM_RELEASE);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for arena base");
        exit(1);
    }

    arena->base = base;
    arena->size = size;
    arena->offset = 0;

    return arena;
}

void ArenaDestroy(Arena* arena) {
    VirtualFree(arena->base, 0, MEM_RELEASE);
    VirtualFree(arena, 0, MEM_RELEASE);
}

void* ArenaPush(Arena* arena, size_t size, size_t alignment) {
    size_t padding = (alignment - (arena->offset % alignment)) % alignment;
    size_t total_size = size + padding;

    if (arena->offset + total_size > arena->size) {
        SDL_Log("Arena out of memory\n");
        exit(1);
    }

    void* block = (void*)((uint8_t*)arena->base + arena->offset + padding);
    arena->offset += total_size;
    return block;
}

char* ArenaPushString(Arena* arena, const char* string) {
    size_t length = strlen(string) + 1;
    char* block = (char*)ArenaPush(arena, length, 1);
    if (block == NULL) {
        SDL_Log("Failed to push string to arena");
        exit(1);
    }
    memcpy(block, string, length);
    return block;
}

void ArenaReset(Arena* arena) {
    memset((uint8_t*)arena->base, 0, arena->size);
    arena->offset = 0;
}
