#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

static SDL_Surface* window;
static int looping;
static direction_t direction = NONE;
volatile int timerStopped = 0;
static TTF_Font* font;

static Uint32 getColor(cell_t c)
{
#define steps 10
#define step (256/steps)
    if(!c) return (128 << 16) | (128 << 8) | 128;
    if(c <= steps) return (255 << 16) | ( (255 - c * step) << 8 ) | 0;
    else return 255 << 16;
#undef step
#undef steps
}

void text(size_t i, size_t j, cell_t c)
{
    char s[10];
    size_t len;

    if(!c) return;

    sprintf(s, "%d", 1 << ((int)c));
    len = strlen(s);

    SDL_Color color = { 255, 255, 255, 0 };
    SDL_Surface* string = TTF_RenderText_Solid(font, s, color);
    SDL_Rect rect = { j * 100 + 25, i * 100 + 25, 50, 50 };
    SDL_BlitSurface(string, NULL, window, &rect);
    SDL_FreeSurface(string);
}

static void draw()
{
    size_t i, j;
    SDL_Rect rect = { 0, 0, 100, 100 };
    for(i = 0; i < 4; ++i) {
        rect.y = i * 100;
        for(j = 0; j < 4; ++j) {
            rect.x = j * 100;
            SDL_FillRect(window, &rect, getColor(board[i * 4 + j]));
            text(i, j, board[i * 4 + j]);
        }
    }
}

static SDLCALL Uint32 ontimer(Uint32 interval)
{
    if(direction != NONE) {
        if(shift[direction]()) addRandomTile();
        direction = NONE;
    }
    draw();
    SDL_UpdateRect(window, 0, 0, 0, 0);
    SDL_Flip(window);
    if(!looping) return 0;
    else return interval;
}

void init_display(int* argc, char* argv[])
{
    SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO);
    TTF_Init();

    font = TTF_OpenFont("FreeMono.ttf", 36);
    if(!font) {
        SDL_Quit();
        abort();
    }

    window = SDL_SetVideoMode(400, 400, 24, SDL_HWSURFACE);
    SDL_WM_SetCaption("2048", "2048");

    SDL_SetTimer(17, &ontimer);

    looping = 0;
}

void loop()
{
    looping = 1;

    while(looping) {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                looping = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    looping = 0;
                    break;
                case SDLK_UP:
                    direction = UP;
                    break;
                case SDLK_LEFT:
                    direction = LEFT;
                    break;
                case SDLK_DOWN:
                    direction = DOWN;
                    break;
                case SDLK_RIGHT:
                    direction = RIGHT;
                    break;
                }
                break;
            }
        }
    }

    SDL_SetTimer(0, NULL);
    SDL_Delay(100); // HOW MUCH TIME DO YOU NEED?!?!?! //SDL_Delay(20); // why? :'(
    SDL_FreeSurface(window);
    TTF_Quit();
    SDL_Quit();
}
