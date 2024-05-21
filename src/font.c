#include "font.h"

FontAtlas* BakeFontAtlas(Arena* arena, FileBuffer font_buffer) {
    FontAtlas* atlas = ArenaPushStruct(arena, FontAtlas);
    atlas->pixels = ArenaPush(arena, FONT_BITMAP_SIZE, alignof(uint8_t));

    if (atlas->pixels == NULL) {
        ExitWithError("Failed to allocate memory for font atlas");
    }

    uint32_t bake_result = stbtt_BakeFontBitmap(font_buffer.base,
                                                0, FONT_PIXEL_HEIGHT,
                                                atlas->pixels,
                                                FONT_BITMAP_WIDTH,
                                                FONT_BITMAP_HEIGHT,
                                                32, 96,
                                                atlas->char_data);
    if (bake_result == 0) {
        ExitWithError("Failed to bake font atlas");
    }

    atlas->width = FONT_BITMAP_WIDTH;
    atlas->height = FONT_BITMAP_HEIGHT;

    return atlas;
}

void WriteFontAtlas(FontAtlas atlas, const char* path) {
    uint32_t result = stbi_write_png(path, atlas.width, atlas.height, 1, atlas.pixels, atlas.width);
    if (result == 0) {
        SDL_Log("Failed to write font atlas to file: %s", path);
    }
}

FontAtlas* LoadDefaultFont(Arena* arena) {
    Arena *temp_arena = ArenaInit(Megabytes(4));
    FileBuffer temp_font_buffer = LoadFile(temp_arena, "../assets/fonts/Roboto-Regular.ttf");

    FontAtlas* atlas = BakeFontAtlas(arena, temp_font_buffer);
    if (temp_font_buffer.size == 0 || atlas->pixels == NULL) {
        ArenaDestroy(temp_arena);
        ExitWithError("Failed to load font");
    }

    // Write 0 pixel as white
    atlas->pixels[0] = 255;

    WriteFontAtlas(*atlas, "font.png");

    ArenaDestroy(temp_arena);
    return atlas;
}

int32_t GetTextWidth(FontAtlas* atlas, const char* text, int32_t length) {
    int32_t response = 0;
    for (const char* char_ptr = text; *char_ptr && length--; char_ptr++) {
        // Skip over UTF-8 continuation bytes
        if ((*char_ptr & 0xc0) == 0x80) {
            continue;
        }

        int32_t char_index = (uint32_t)*char_ptr - 32;
        if (char_index >= 0 && char_index < 96) {
            response += atlas->char_data[char_index].xadvance;
        }
    }
    return response;
}

int32_t GetTextHeight(void) {
    return FONT_PIXEL_HEIGHT;
}

