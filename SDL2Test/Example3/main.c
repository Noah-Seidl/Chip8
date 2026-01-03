#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#define WINDOW_TITLE "TEST"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define FONT_SIZE 80


struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *text_font;
    SDL_Color text_color;
    SDL_Rect text_rect;
    SDL_Texture *text_image;
};

void drawText(struct Game *game, char *text);
void game_cleanup(struct Game *game, int exit_status);
bool sdl_initialze(struct Game *game);

int main()
{
    struct Game game =
    {
        .window = NULL,
        .renderer = NULL,
        .text_font = NULL,
        .text_color = {255,0,0,255},
        .text_rect = {0, 50, 0, 0},
        .text_image = NULL
    };
    
    if(sdl_initialze(&game))
    {
        game_cleanup(&game,EXIT_FAILURE);
    }



    while (true) 
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                game_cleanup(&game, EXIT_SUCCESS);
                break;
            
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    game_cleanup(&game, EXIT_SUCCESS);
                    break;
                
                default:
                    break;
                }
                break;

            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(game.renderer,255,255,255,255);
        SDL_RenderClear(game.renderer);

        SDL_SetRenderDrawColor(game.renderer,255,0,255,255);

        SDL_RenderDrawPoint(game.renderer, SCREEN_WIDTH /2, SCREEN_HEIGHT /2);

        //Text schreiben:

        drawText(&game, "Noahs Emulator");

        SDL_RenderCopy(game.renderer, game.text_image, NULL, &game.text_rect);

        SDL_RenderPresent(game.renderer);

        SDL_Delay(16);    
    }
    




    printf("Hello World\n");
    game_cleanup(&game, EXIT_SUCCESS);
    return 0;
} 

void drawText(struct Game *game, char *text)
{
    game->text_font = TTF_OpenFont("freesansbold.ttf", FONT_SIZE);
    SDL_Surface *surface = TTF_RenderText_Blended(game->text_font, text, game->text_color);
    game->text_rect.w = surface->w;
    game->text_rect.h = surface->h;
    game->text_image = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

}


void game_cleanup(struct Game *game, int exit_status)
{
    
    SDL_DestroyTexture(game->text_image);
    TTF_CloseFont(game->text_font);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    TTF_Quit();
    SDL_Quit();
    exit(exit_status);
}

bool sdl_initialze(struct Game *game)
{
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        fprintf(stderr, "SDLINIT failed %s", SDL_GetError());
        return true;    
    }

    if(TTF_Init())
    {
        fprintf(stderr, "WINDOW Creating failed %s", SDL_GetError());
        return true;   
    }


    game->window = SDL_CreateWindow(WINDOW_TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!game->window)
    {
        fprintf(stderr, "WINDOW Creating failed %s", SDL_GetError());
        return true;   
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if(!game->renderer)
    {
        fprintf(stderr, "Renderer Creating failed %s", SDL_GetError());
        return true;   
    }


    return false;
}