#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
//#include "GLSL_helper.h"
#include <math.h>
#include "Enemy.h"

#define DEGRAD 3.1415926 / 180
#define VELOCITY 0.8

// Portable version of system("PAUSE")
void waitForUser() 
{
	std::cout << "Press [Enter] to continue . . .";
	std::cin.get();
}

Enemy::Enemy (float random)
{
	srand(random);

	Translation.X = rand() % 9 + 0.5;
	Translation.Y = 0.5;
	Translation.Z = rand() % 9 + 0.5;

	Scale.X = 1; 
	Scale.Y = 1;
	Scale.Z = 1;

	Rotation.Z = 0;
	Rotation.X = 0;
	Rotation.Y = rand() % 360;
	alive = 1;


	size = 0.38;

	next = 0;

	// First create a shader loader and check if our hardware supports shaders
	CShaderLoader ShaderLoader;
	if (! ShaderLoader.isValid())
	{
		std::cerr << "Shaders are not supported by your graphics hardware, or the shader loader was otherwise unable to load." << std::endl;
		waitForUser();
	}

	// Now attempt to load the shaders
	Shader = ShaderLoader.loadShader("Shaders/Lab3_vert.glsl", "Shaders/Lab3_frag.glsl");
	if (! Shader)
	{
		std::cerr << "Unable to open or compile necessary shader." << std::endl;
		waitForUser();
	}
	Shader->loadAttribute("aPosition");
	Shader->loadAttribute("aColor");
        Shader->loadAttribute("aNormal");

	// Now attempt to load the shaders
	Shader2 = ShaderLoader.loadShader("Shaders/Lab3_vert2.glsl", "Shaders/Lab3_frag.glsl");
	if (! Shader2)
	{
		std::cerr << "Unable to open or compile necessary shader." << std::endl;
		waitForUser();
	}
	Shader2->loadAttribute("aPosition");
	Shader2->loadAttribute("aColor");
        Shader2->loadAttribute("aNormal");
	

	// Attempt to load mesh
	CMesh * Mesh = CMeshLoader::loadASCIIMesh("Models/gargoyle500.m");
	if (! Mesh)
	{
		std::cerr << "Unable to load necessary mesh." << std::endl;
		waitForUser();
	}
	// Make out mesh fit within camera view
	Mesh->resizeMesh(SVector3(1));
	// And center it at the origin
	Mesh->centerMeshByExtents(SVector3(0));

	// Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
	CMeshLoader::createVertexBufferObject(* Mesh, TriangleCount, PositionBufferHandle, ColorBufferHandle, NormalBufferHandle);

}

Enemy::~Enemy(){
	
}


void Enemy::fixRotate (char val, int place)
{
	while (Rotation.Y > 180)
	{
		Rotation.Y -= 180;
	}
	while (Rotation.Y < -180)
	{
		Rotation.Y += 180;
	}


	if (val == 'x')
	{
		if (place == 0)
		{
			if (Rotation.Y > 90 || Rotation.Y < -90)
			{
				Rotation.Y = -Rotation.Y;
			}
			else
			{
				Rotation.Y = -Rotation.Y;
			}
			Translation.X = -Translation.X;
		}
		else
		{
			if (Rotation.Y > 90 || Rotation.Y < -90)
			{
				Rotation.Y = -Rotation.Y;
			}
			else
			{
				Rotation.Y = -Rotation.Y;
			}
			Translation.X = 10 - (Translation.X - 10);
		}
	}
	if (val == 'z')
	{
		if (place == 0)
		{
			if (Rotation.Y > 0)
			{
				Rotation.Y = 180 - Rotation.Y;
			}
			else
			{
				Rotation.Y = -180 - Rotation.Y;
			}
			Translation.Z = -Translation.Z;
		}
		else
		{
			if (Rotation.Y > 0)
			{
				Rotation.Y = 180 - Rotation.Y;
			}
			else
			{
				Rotation.Y = Rotation.Y + 180;
			}
			Translation.Z = 10 - (Translation.Z - 10);
		}
	}
}

void Enemy::update(float time)
{
	if (alive)
	{
		Translation.X += VELOCITY * time * sin(Rotation.Y * DEGRAD);
		Translation.Z += VELOCITY * time * cos(Rotation.Y * DEGRAD);

		if (Translation.X < 0)  { fixRotate('x', 0);  }
		if (Translation.X > 10) { fixRotate('x', 10); }
		if (Translation.Z < 0)  { fixRotate('z', 0);  }
		if (Translation.Z > 10) { fixRotate('z', 10); }
	}
}

void Enemy::draw()
{
	if (!alive)
	{
		// Shader context works by cleaning up the shader settings once it
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

		glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);

		glPopMatrix();
	}
	else
	{
		// Shader context works by cleaning up the shader settings once it
		// goes out of scope
		CShaderContext ShaderContext(* Shader2);
		ShaderContext.bindBuffer("aPosition", PositionBufferHandle, 4);
		ShaderContext.bindBuffer("aColor", ColorBufferHandle, 3);
                ShaderContext.bindBuffer("aNormal", NormalBufferHandle, 3);

		glPushMatrix();

		glTranslatef(Translation.X, Translation.Y, Translation.Z);
		glRotatef(Rotation.Z, 0, 0, 1);
		glRotatef(Rotation.Y, 0, 1, 0);
		glRotatef(Rotation.X, 1, 0, 0);
		glScalef(Scale.X, Scale.Y, Scale.Z);

		glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);

		glPopMatrix();
	}
}
