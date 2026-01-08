#include "SDLWindow.h"

//SDL Cleanup funktion
void cleanup(Window *window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}

//SDL init
void initSDL(Window *window)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    window->renderer = SDL_CreateRenderer(window->window,-1,0);
}