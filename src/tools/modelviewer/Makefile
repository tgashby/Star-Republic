# Simple makefile for lab1
# Andrew J. Musselman

NAME = modelviewer
CFLAGS = $(shell sdl-config --cflags) -g -Wall  
LDFLAGS = -lSDL -lGL -lGLU  
CC = g++
OBJS = ApplicationEngine.o Camera.o main.o \
	Mesh.o Object3d.o  RenderingEngine.o ResourceManager.o \
	MeshParser.o




all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(NAME)

ApplicationEngine.o: ApplicationEngine.cpp
	$(CC) $(CFLAGS) -c ApplicationEngine.cpp 

Camera.o:Camera.cpp Camera.h
	$(CC) $(CFLAGS) -c Camera.cpp Camera.h 

Mesh.o: Mesh.cpp Meshes.h
	$(CC) $(CFLAGS) -c Mesh.cpp Meshes.h 

MeshParser.o: MeshParser.cpp MeshParser.h
	$(CC) $(CFLAGS) -c MeshParser.cpp MeshParser.h

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
