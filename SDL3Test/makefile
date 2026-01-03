CFLAGS=-O3 -MMD -MP -Wall #-fsanitize=address
LDFLAGS=-lSDL3
PROGRAMS=programm
OBJECTS=main.o

ALLOBJECTS=$(OBJECTS) $(patsubst %,%.o, $(PROGRAMS))

default: $(PROGRAMS)

clean:
	-rm -f $(ALLOBJECTS) $(ALLOBJECTS:%.o=%.d) $(PROGRAMS)

programm: $(OBJECTS)
	gcc $(CFLAGS) $(LDFLAGS) -o programm $(OBJECTS)

-include $(ALLOBJECTS:%.o=%.d)