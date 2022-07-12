#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"

SDL_Window* window;
SDL_Renderer* renderer;

#define COLOR(c)                                                     \
    (((c) >> 24) & 0xff), (((c) >> 16) & 0xff), (((c) >> 8) & 0xff), \
        (((c) >> 0) & 0xff)

void platform_rect(float x, float y, float w, float h, size_t color) {
    SDL_SetRenderDrawColor(renderer, COLOR(color));
    SDL_RenderFillRectF(renderer, &(SDL_FRect){x, y, w, h});
}

void platform_print(char* txt) { printf("%s\n", txt); }

float platform_frand(void) { return rand() / (float)RAND_MAX; }

typedef struct {
    SDL_Texture* tex;
    SDL_Rect rect;
} TextureSlice;

#define MAX_TEXTURES 128
TextureSlice textures[MAX_TEXTURES];
size_t texture_count = 0;

size_t platform_load_texture(char* name) {
    char buff[256];
    {
        char* en = buff;
        strcpy(en, "assets/");
        en += strlen("assets/");
        strcpy(en, name);
        en += strlen(name);
        *en = '\0';
    }
    printf("[INFO] loading: %s\n", buff);

    SDL_Surface* surf = SDL_LoadBMP(buff);

    if (!surf) {
        fprintf(stderr, "[ERROR] Failed to load %s, due to %s\n", buff,
                SDL_GetError());
        exit(1);
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    SDL_Rect rect = {};

    SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);

    textures[texture_count].tex = tex;
    textures[texture_count].rect = rect;
    texture_count++;

    return texture_count - 1;
}

void platform_blit(float x, float y, float w, float h, size_t tex) {
    SDL_FRect rect = {
        .x = x,
        .y = y,
        .w = w ? w : textures[tex].rect.w,
        .h = h ? h : textures[tex].rect.h,
    };
	
    SDL_RenderCopyF(renderer, textures[tex].tex, &textures[tex].rect, &rect);
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "[ERROR] SDL failed to init %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 1280, 720,
                              SDL_WINDOW_ALWAYS_ON_TOP);

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = 1;
    size_t last_time = SDL_GetTicks();

    game_init();

    size_t last_report = SDL_GetTicks();
    size_t frames = 0;

    while (running) {
        size_t now_time = SDL_GetTicks();
        float dt = (now_time - last_time) * 0.001f;
        last_time = now_time;
        if (now_time - last_report >= 1000) {
            printf("FPS: %zu\n", frames);
            frames = 0;
            last_report = now_time;
        }
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT: {
                    running = 0;
                } break;
                case SDL_KEYDOWN: {
                    game_key_down(ev.key.keysym.sym);
                } break;
                case SDL_KEYUP: {
                    game_key_up(ev.key.keysym.sym);
                } break;
                case SDL_MOUSEMOTION: {
                    game_mouse_move(ev.motion.x, ev.motion.y);
                }
            }
        }

        // update
        game_update(dt);

        SDL_SetRenderDrawColor(renderer, COLOR(0x111111ff));
        SDL_RenderClear(renderer);

        // render
        game_render();
        frames++;

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
}
