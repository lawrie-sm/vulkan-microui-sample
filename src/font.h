#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#include "defines.h"
#include "arena.h"
#include "file.h"
#include "platform.h"

#define FONT_BITMAP_WIDTH 256
#define FONT_BITMAP_HEIGHT 256
#define FONT_BITMAP_SIZE (FONT_BITMAP_WIDTH * FONT_BITMAP_HEIGHT)
#define FONT_PIXEL_HEIGHT 16

// This can be calculated from offset per font, but this is fine
#define FONT_MAX_ASCENT 15

typedef struct {
    stbtt_bakedchar char_data[96];
    uint8_t* pixels;
    uint32_t width;
    uint32_t height;
} FontAtlas;


FontAtlas* LoadDefaultFont(Arena* arena);
void WriteFontAtlas(FontAtlas atlas, const char* path);

int32_t GetTextWidth(FontAtlas* atlas, const char* text, int32_t length);
int32_t GetTextHeight(void);
