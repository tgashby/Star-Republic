#include <iostream>

#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#include <stdio.h>

#include "Player.h"

void waitForUser3() 
{
	std::cout << "Press [Enter] to continue . . .";
	std::cin.get();
}

Player::Player(SVector3* pos, SVector3* vel, CMesh* mod, float size) : GameObject(pos, vel, mod, size) {
	 
  int health = 100;
  int cooldown = 0;
  bool firing = false;

  Translation.X = pos->X;
	Translation.Y = pos->Y;
	Translation.Z = pos->Z;

	Scale.X = 1; 
	Scale.Y = 1;
	Scale.Z = 1;

	Rotation.X = 0;
	Rotation.Y = 90;
	Rotation.Z = 0;

  // First create a shader loader and check if our hardware supports shaders
	CShaderLoader ShaderLoader;
	if (! ShaderLoader.isValid())
	{
		std::cerr << "Shaders are not supported by your graphics hardware, or the shader loader was otherwise unable to load." << std::endl;
		waitForUser3();
	}

	// Now attempt to load the shaders
	shade = ShaderLoader.loadShader("Shaders/GameVert2.glsl", "Shaders/Lab3_frag.glsl");
	if (! shade)
	{
		std::cerr << "Unable to open or compile necessary shader." << std::endl;
		waitForUser3();
	}
	shade->loadAttribute("aPosition");
	shade->loadAttribute("aColor");
  shade->loadAttribute("aNormal");
	
	// Attempt to load mesh
	mod = CMeshLoader::loadASCIIMesh("Models/cessna_color500.m");
	if (! mod)
	{
		std::cerr << "Unable to load necessary mesh." << std::endl;
		waitForUser3();
	}
	// Make out mesh fit within camera view
	mod->resizeMesh(SVector3(1));
	// And center it at the origin
	mod->centerMeshByExtents(SVector3(0));

	// Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
	CMeshLoader::createVertexBufferObject(* mod, TriangleCount, 
		PositionBufferHandle, ColorBufferHandle, NormalBufferHandle);
}

Player::~Player()
{ }
#pragma once
void Player::setRefx(float rx)
{
   refx = rx;
}
void Player::setRefy(float ry)
{
   refy = ry;
}

SVector3* Player::getPosition()
{
   return position;
}


void Player::update(float dt, Map* bounds)
{
        velocity->X = -(refx - position->X);
        velocity->Y = -(refy - position->Y);

	position->X += velocity->X * dt;
	position->Y += velocity->Y * dt;
	position->Z += velocity->Z * dt;
	
	Rotation.X = velocity->X * 5.0;
	Rotation.Y = 90 + velocity->Y * 5.0;

	Translation.X = position->X;
	Translation.Y = position->Y;
	Translation.Z = position->Z;
}
void Player::draw()
{
	{
		// Shader context works by cleaning up the shader settings once it
		// goes out of scope
		CShaderContext ShaderContext(*shade);
		ShaderContext.bindBuffer("aPosition", PositionBufferHandle, 4);
		ShaderContext.bindBuffer("aColor", ColorBufferHandle, 3);
		ShaderContext.bindBuffer("aNormal", NormalBufferHandle, 3);

		glPushMatrix();

		glTranslatef(Translation.Y + 6, Translation.X + 4, Translation.Z);
		glRotatef(Rotation.X, 1, 0, 0);
		glRotatef(Rotation.Y, 0, 1, 0);
		glScalef(Scale.X, Scale.Y, Scale.Z);

		glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);

		glPopMatrix();
	}
}
void Player::collideWith(GameObject collided)
{

}


