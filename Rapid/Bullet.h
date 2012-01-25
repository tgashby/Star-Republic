#pragma once
#include "GameObject.h"
#include <typeinfo>

#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"
#include "Util/SVector3.h"

#endif

class Bullet : GameObject
{
public:

   CShader* shade;
   int damage;
   int toDie;

   // Handles for VBOs
   GLuint PositionBufferHandle, ColorBufferHandle, NormalBufferHandle;

   // Information about mesh
   SVector3 Translation, Rotation, Scale;
   int TriangleCount;
   
   Bullet(SVector3* pos, SVector3* vel, CMesh* mod, float size, int damage);
   ~Bullet();
   void update(float dt);
   void draw();
   void collideWith(GameObject* collided);
   SVector3* getPosition();
   int gettoDie();
};
