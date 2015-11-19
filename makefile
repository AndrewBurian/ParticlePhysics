APPNAME=particlesim
LIBS=-lm -lSDL2 -lSDL2_ttf
LDFLAGS=
CCFLAGS=-c -g -Werror -Wall

all: $(APPNAME)

$(APPNAME): universe.o simulator.o physics.o render.o input.o
	$(CC) $(LDFLAGS) universe.o simulator.o physics.o render.o input.o -o $(APPNAME) $(LIBS)

run: $(APPNAME)
	./$(APPNAME)

universe.o: universe.c universe.h
	$(CC) $(CCFLAGS) universe.c

simulator.o: simulator.c universe.h
	$(CC) $(CCFLAGS) simulator.c

physics.o: physics.c universe.h
	$(CC) $(CCFLAGS) physics.c

render.o: render.c universe.h
	$(CC) $(CCFLAGS) render.c

input.o: input.c simulator.h
	$(CC) $(CCFLAGS) input.c

clean:
	rm $(APPNAME) *.o
