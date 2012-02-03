NAME = a.out
CFLAGS = $(shell sdl-config --cflags) -g -Wall  
LDFLAGS = -lSDL -lGL -lGLU  
CC = g++
OBJS = GameEngine.o Camera.o Main.o \
	Mesh.o Object3d.o  RenderingEngine.o ResourceManager.o




all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(NAME)

GameEngine.o: GameEngine.cpp
	$(CC) $(CFLAGS) -c GameEngine.cpp 

Camera.o:Camera.cpp Camera.h
	$(CC) $(CFLAGS) -c Camera.cpp Camera.h 

Main.o:Main.cpp
	$(CC) $(CFLAGS) -c Main.cpp

Mesh.o: Mesh.cpp Mesh.h
	$(CC) $(CFLAGS) -c Mesh.cpp Mesh.h 

Object3d.o: Object3d.cpp 
	$(CC) $(CFLAGS) -c Object3d.cpp 

RenderingEngine.o: RenderingEngine.cpp
	$(CC) $(CFLAGS) -c RenderingEngine.cpp 

ResourceManager.o: ResourceManager.cpp
	$(CC) $(CFLAGS) -c ResourceManager.cpp 

Vector.o: Vector.cpp
	$(CC) $(CFLAGS) -c Vector.cpp 

 
.phony : clean 
clean:
	rm *.o 
	rm *.gch
	rm game
