#pragma once
#include "GameObject.h"

#include <typeinfo>

#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"

#endif

#include "Map.h"

class Player : GameObject
{
public:

   int health;
   int cooldown;
   bool firing;
   CShader* shade;
   float refx, refy; // for movement

   // Handles for VBOs
   GLuint PositionBufferHandle, ColorBufferHandle, NormalBufferHandle;

   // Information about mesh
   SVector3 Translation, Rotation, Scale;
   int TriangleCount;

   Player(SVector3* pos, SVector3* vel, CMesh * mod, float size);
   ~Player();
   void update(float dt, Map* m);
   void draw();
   void setRefx(float rx);
   void setRefy(float ry);
   void setFiring(bool state);
   bool canFire();
   void speedUp();
   void slowDown();
   float getSize();
   void collideWith(GameObject collided);
   SVector3* getPosition();
   SVector3* getTranslation();
   SVector3* getVelocity();
};
