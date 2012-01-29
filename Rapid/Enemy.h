#pragma once

#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"

#endif

class Enemy{
public: 
	int alive;
	float velocity;
	Enemy* next;
	float size;

	// Shader and Mesh utility classes
	CShader * Shader;
	CShader * Shader2;
	CMesh * Mesh;

	// Handles for VBOs
	GLuint PositionBufferHandle, ColorBufferHandle, NormalBufferHandle;

	// Information about mesh
	SVector3 Translation, Rotation, Scale;
	int TriangleCount;

	Enemy (float random);
	~Enemy();
	void draw ();
	void fixRotate (char val, int place);
	void update(float time);
};
