#include "chip8Display.h"

void renderDisplay(Window *window, Chip8 *chip);


void chip8Display(Window *window, char *filename)
{
    //Chip init
    Chip8 chip = initChip8(filename);

    //hier wird der key gespeichert kann nur einen einzigen key an den chip übergebn
    int key = 0;

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            //Quit Event wird abgefragt
            case SDL_QUIT:
                cleanup(window);
                break;

            //Auslesung der Keys
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_SPACE:
                        key = 1;
                        break;

                    case SDL_SCANCODE_A:
                        key = 5;
                        break;

                    case SDL_SCANCODE_Q:
                        key = 4;
                        break;
                    case SDL_SCANCODE_D:
                        key = 6;
                        break;
                    

                    default:
                        break;
                }
                
            default:
                break;
            }

        }        
        //wäre auch besser wenn man nur immer den display updatet wenn wirklich das display sich auch im chip verändert
        //ansonsten kann man ja befehle nur alle 60 mal pro sekunde aufrufen 
        //also eigentlich am besten schleife in chip8Display execute und sie returnt nur bei display changes?
        //Chip befehlt befehl mit key übergabe au
        execute(&chip, key);

        //key muss auf nichtbenutzen wert gesetz werden da sonst sozusagen der key solange kein anderer gedrückt wird göeich bleibt
        key = -1;

        //lät dass Display des chip8 auf das sdlwindow mit beliebiger vergrößerung
        renderDisplay(window,&chip);

        SDL_RenderPresent(window->renderer);
 


        SDL_Delay(16);
    }
    
}


void renderDisplay(Window *window, Chip8 *chip)
{
    int x = 0,y = 0;

    for (size_t i = 0; i < 64*32; i++)
    {
        x = i % 64;
        y = i / 64;
        SDL_Rect rect; //es wird rect genutzt um einen Pixel als mehrere Pixel reinzuzeichnen um einen pixel auf Display size zu vergrößern
        rect.x = x * DISPLAY_SIZE;
        rect.y = y * DISPLAY_SIZE;
        rect.h = DISPLAY_SIZE;
        rect.w = DISPLAY_SIZE;

        if(chip->display[i]) //hat nur zwei farben
            SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);
        else
            SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);

        SDL_RenderFillRect(window->renderer, &rect);
    }
}