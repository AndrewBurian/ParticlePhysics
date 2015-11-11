APPNAME=particlesim
LIBS=-lm -lSDL2
LDFLAGS=-g
CCFLAGS=-c

all: $(APPNAME)

$(APPNAME): universe.o simulator.o physics.o render.o
	$(CC) $(LDFLAGS) universe.o simulator.o physics.o render.o -o $(APPNAME) $(LIBS)

universe.o: universe.c universe.h
	$(CC) $(CCFLAGS) universe.c

simulator.o: simulator.c universe.h
	$(CC) $(CCFLAGS) simulator.c

physics.o: physics.c universe.h
	$(CC) $(CCFLAGS) physics.c

render.o: render.c universe.h
	$(CC) $(CCFLAGS) render.c

clean:
	rm $(APPNAME) *.o
