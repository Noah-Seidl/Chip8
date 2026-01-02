#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "TEST"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000



struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;

};

void game_cleanup(struct Game *game, int exit_status);
bool sdl_initialze(struct Game *game);

int main()
{
    struct Game game =
    {
        .window = NULL,
        .renderer = NULL,
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
        SDL_RenderClear(game.renderer);
        
        SDL_RenderPresent(game.renderer);

        SDL_Delay(16);    
    }
    




    printf("Hello World\n");
    game_cleanup(&game, EXIT_SUCCESS);
    return 0;
} 

void game_cleanup(struct Game *game, int exit_status)
{
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
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