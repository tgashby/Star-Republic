#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include <GL\glew.h>
#include <GL\glut.h>
#endif

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
//#include "GLSL_helper.h"
#include <math.h>
#include "Map.h"

// Portable version of system("PAUSE")
void waitForUserTwo() 
{
	std::cout << "Press [Enter] to continue . . .";
	std::cin.get();
}

Map::Map ()
{
	Translation.X = 0;
	Translation.Y = 0;
	Translation.Z = 0;

	Scale.X = 1; 
	Scale.Y = 1;
	Scale.Z = 1;

	Rotation.Z = 0;
	Rotation.X = 0;
	Rotation.Y = 0;

        xmin = -2.2;
        xmax = 2.2;
        ymin = -1.9;
        ymax = 1.9;

        
	groundxmin = 1;
        groundxmax = 11;
        groundzmin = 45;
        groundzmax = 500;


	// First create a shader loader and check if our hardware supports shaders
	CShaderLoader ShaderLoader;
	if (! ShaderLoader.isValid())
	{
		std::cerr << "Shaders are not supported by your graphics hardware, or the shader loader was otherwise unable to load." << std::endl;
		waitForUserTwo();
	}

	// Now attempt to load the shaders
	Shader = ShaderLoader.loadShader("Shaders/GameVert1.glsl", "Shaders/Lab3_frag.glsl");
	if (! Shader)
	{
		std::cerr << "Unable to open or compile necessary shader." << std::endl;
		waitForUserTwo();
	}
	Shader->loadAttribute("aPosition");
	Shader->loadAttribute("aColor");
	Shader->loadAttribute("aNormal");

	// Attempt to load mesh
	//CMesh * Mesh = CMeshLoader::loadASCIIMesh("Models/canyon.obj");
	CMesh * Mesh = CMeshLoader::loadASCIIMesh("Models/grid.m");
	if (! Mesh)
	{
		std::cerr << "Unable to load necessary mesh." << std::endl;
		waitForUserTwo();
	}
	// Make out mesh fit within camera view
	/*Mesh->resizeMesh(SVector3(1));
	// And center it at the origin
	Mesh->centerMeshByExtents(SVector3(0));*/

	// Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
	CMeshLoader::createVertexBufferObject(* Mesh, TriangleCount, PositionBufferHandle, ColorBufferHandle, NormalBufferHandle);

	printf("*************\n");
}

Map::~Map(){
	
}


void Map::draw()
{	
	{
		// Shader context works 
		// by cleaning up the shader settings once it
		// goes out of scope
		CShaderContext ShaderContext(* Shader);
		ShaderContext.bindBuffer("aPosition", PositionBufferHandle, 4);
		ShaderContext.bindBuffer("aColor", ColorBufferHandle, 3);
                ShaderContext.bindBuffer("aNormal", NormalBufferHandle, 3);

		glPushMatrix();

		glTranslatef(Translation.X, Translation.Y, Translation.Z);
		glRotatef(Rotation.Z, 0, 0, 1);
		glRotatef(Rotation.Y, 0, 1, 0);
		glRotatef(Rotation.X, 1, 0, 0);
		glScalef(Scale.X, Scale.Y, Scale.Z);

		glDrawArrays(GL_TRIANGLES, 0, 3 * TriangleCount);

		glPopMatrix();
	}
}
