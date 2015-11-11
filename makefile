APPNAME=particlesim
LIBS=-lSDL2
LDFLAGS=-g
CCFLAGS=-c

all: $(APPNAME)

$(APPNAME): simulator.o physics.o render.o
	$(CC) $(LDFLAGS) simulator.o physics.o render.o -o $(APPNAME) $(LIBS)

simulator.o: simulator.c universe.h
	$(CC) $(CCFLAGS) simulator.c

physics.o: physics.c universe.h
	$(CC) $(CCFLAGS) physics.c

render.o: render.c universe.h
	$(CC) $(CCFLAGS) render.c

clean: 
	rm $(APPNAME) *.o
