#include "ui.h"

int32_t UIGetTextWidth(mu_Font font, const char *text, int32_t length) {
    if (length == -1) {
        length = strlen(text);
    }
    return GetTextWidth((FontAtlas *)font, text, length);
}

int32_t UIGetTextHeight(mu_Font font) {
    return GetTextHeight();
}

UI *UIInit(Arena *arena, FontAtlas *atlas) {
    UI *ui = ArenaPushStruct(arena, UI);
    mu_init(&ui->ctx);

    ui->ctx.style->font = (mu_Font)atlas;
    ui->ctx.text_width = UIGetTextWidth;
    ui->ctx.text_height = UIGetTextHeight;

    return ui;
}

bool UIProcessInputs(UI *ui) {
    mu_Context *ctx = &ui->ctx;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: {
                return false;
            } break;

            case SDL_MOUSEMOTION: {
                mu_input_mousemove(ctx, e.motion.x, e.motion.y);
            } break;
            case SDL_MOUSEWHEEL: {
                mu_input_scroll(ctx, 0, e.wheel.y * -30);
            } break;

            case SDL_TEXTINPUT: {
                mu_input_text(ctx, e.text.text);
            } break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                int32_t btn = button_map[e.button.button & 0xff];
                if (btn && e.type == SDL_MOUSEBUTTONDOWN) {
                    mu_input_mousedown(ctx, e.button.x, e.button.y, btn);
                }
                if (btn && e.type == SDL_MOUSEBUTTONUP) {
                    mu_input_mouseup(ctx, e.button.x, e.button.y, btn);
                }
            } break;

            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return false;
                }

                int32_t key = key_map[e.key.keysym.sym & 0xff];
                if (key && e.type == SDL_KEYDOWN) {
                    mu_input_keydown(ctx, key);
                }
                if (key && e.type == SDL_KEYUP) {
                    mu_input_keyup(ctx, key);
                }
            } break;
        }
    }

    return true;
}

void UIUpdate(UI *ui) {
    mu_Context *ctx = &ui->ctx;

    mu_begin(ctx);

    if (mu_begin_window(ctx, "My Test Window", mu_rect(5, 5, 300, 300))) {
        if (mu_button(ctx, "Button")) { 
            SDL_Log("Button pressed");
        }
        mu_end_window(ctx);
    }

    mu_end(ctx);
}
