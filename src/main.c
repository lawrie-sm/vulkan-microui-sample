#include "main.h"

int x[100];
int main(int argc, char* argv[]) {
    SDL_Log("Init\n");

    VkExtent2D extent = {1680, 1050};

    Arena* main_arena = ArenaInit(Megabytes(8));

    Platform* platform = PlatformInit(main_arena, extent);
    Graphics* gfx = GraphicsInit(main_arena, platform, extent);
    FontAtlas* font_atlas = LoadDefaultFont(main_arena);
    UI* ui = UIInit(main_arena, font_atlas);
    Renderer* renderer = RendererInit(main_arena, gfx, font_atlas);

    uint64_t current_frame = 0;
    Arena* scratch_arena = ArenaInit(Megabytes(8));
    while (true) {
        bool is_running = UIProcessInputs(ui);
        if (!is_running) {
            break;
        }

        UIUpdate(ui);
        RendererDrawUIFrame(scratch_arena, renderer, gfx, ui, font_atlas, current_frame);

        ArenaReset(scratch_arena);
        current_frame++;
    }
    ArenaDestroy(scratch_arena);

    RendererDestroy(renderer, gfx);
    GraphicsDestroy(gfx);
    PlatformDestroy(platform);
    ArenaDestroy(main_arena);

    return 1;
}
