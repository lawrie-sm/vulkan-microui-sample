#pragma once

#include <SDL2/SDL.h>
#include <stdalign.h>
#include <stdint.h>
#include <windows.h>

typedef struct Arena {
    void* base;
    size_t size;
    uint64_t offset;
} Arena;

Arena* ArenaInit(size_t size);
void ArenaDestroy(Arena* arena);
void* ArenaPush(Arena* arena, size_t size, size_t alignment);
char* ArenaPushString(Arena* arena, const char* string);
void ArenaPop(Arena* arena, size_t size);
void ArenaReset(Arena* arena);

#define ArenaPushStruct(arena, struct) \
    (struct*)ArenaPush(arena, sizeof(struct), alignof(struct))

#define ArenaPushArray(arena, array, count) \
    (array*)ArenaPush(arena, sizeof(array) * (count), alignof(array))

#define ArenaPushPrimitive(arena, type) \
    (type*)ArenaPush(arena, sizeof(type), alignof(type))

