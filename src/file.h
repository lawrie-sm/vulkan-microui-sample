#pragma once

#include <stdint.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "arena.h"
#include "defines.h"

typedef struct {
    uint8_t* base;
    size_t size;
} FileBuffer;

FileBuffer LoadFile(Arena* arena, const char* path);
