#pragma once
#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"

#endif

class Map{
public: 

	// Shader and Mesh utility classes
	CShader * Shader;
	CMesh * Mesh;

	// Handles for VBOs
	GLuint PositionBufferHandle, ColorBufferHandle, NormalBufferHandle;

	// Information about mesh
	SVector3 Translation, Rotation, Scale;
	int TriangleCount;

	int xmin, xmax, ymin, ymax;

	Map ();
	~Map();
	void draw ();
};
