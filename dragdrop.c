#include "SDLWindow.h"
#include <stdbool.h>

char *openWindowDragandDrop(Window *window)
{
    while (true)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return NULL;
                break;
            
            //Wird ein string alloziert der größe des filenames dann gespeicher und der event.drop.file string muss gefreed werden
            case SDL_DROPFILE:
                char *result = malloc(strlen(event.drop.file) + 1);
                strcpy(result, event.drop.file);    
                SDL_free(event.drop.file);
                return result; //rückgabe bei drag and drop
            break;

            default:
                break;
            }
        }
        
    //einfarbiges fenster
    SDL_SetRenderDrawColor(window->renderer,255,255,255,255);
    SDL_RenderClear(window->renderer);
    SDL_RenderPresent(window->renderer);


    SDL_Delay(16);
    }
}