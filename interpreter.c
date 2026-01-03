#include <stdio.h>
#include "SDLWindow.h"
#include "dragdrop.h"
#include "test.h"

void readFile(char *filename);

int main()
{    

    Window window;

    initSDL(&window);

    char *filename = openWindowDragandDrop(&window);
    
    printf("%s\n", filename);    


    testWindow(&window);


    return 0;
}

void readFile(char *filename)
{
    FILE *fptr = fopen(filename, "rb");

    if(fptr == NULL)
    return;
    

    


}