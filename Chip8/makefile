CFLAGS=-O0 -MMD -MP -Wall #-fsanitize=address
LDFLAGS=-lSDL2 -lSDL2_ttf
PROGRAMS=programm
OBJECTS=main.o SDLWindow.o dragdrop.o emulator.o chip8Display.o

ALLOBJECTS=$(OBJECTS) $(patsubst %,%.o, $(PROGRAMS))

default: $(PROGRAMS)

clean:
	-rm -f $(ALLOBJECTS) $(ALLOBJECTS:%.o=%.d) $(PROGRAMS)

programm: $(OBJECTS)
	gcc -g $(CFLAGS) $(LDFLAGS) -o programm $(OBJECTS)

-include $(ALLOBJECTS:%.o=%.d)