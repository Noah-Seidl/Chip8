#include <stdio.h>
#include "SDLWindow.h"
#include "dragdrop.h"
#include "chip8Display.h"
#include "emulator.h"

int main()
{    
    Window window;

    initSDL(&window);

    //Wartet bis per drag and drop der filename zurückgegeben wird falls flasch wird programm abgebrochen
    char *filename = openWindowDragandDrop(&window);
    if(filename == NULL){
        cleanup(&window);
        return 1;
    }
    printf("%s\n", filename);    

    
    chip8Display(&window, filename);

    cleanup(&window);

    return 0;
}
