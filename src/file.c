#include "file.h"

FileBuffer LoadFile(Arena* arena, const char* path) {
    FileBuffer block = {0};

    FILE* file;
    errno_t err = fopen_s(&file, path, "rb");
    if (err != 0 || file == NULL) {
        SDL_Log("Failed to open file: %s", path);
        return block;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    if (size == 0) {
        SDL_Log("File is empty: %s", path);
        fclose(file);
        return block;
    }
    block.size = size;

    block.base = ArenaPush(arena, block.size, alignof(uint8_t));
    if (block.base == NULL) {
        SDL_Log("Failed to allocate memory for file: %s", path);
        fclose(file);
        return block;
    }

    fseek(file, 0, SEEK_SET);
    size_t read_size = fread(block.base, 1, block.size, file);
    if (read_size != block.size) {
        SDL_Log("Failed to read file: %s", path);
    }

    return block;
}
