#include "GameObject.h"
#include <typeinfo>

#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"

#endif

class Player : GameObject
{
public:

   int health;
   int cooldown;
   bool firing;
   CShader* shade;

   // Handles for VBOs
   GLuint PositionBufferHandle, ColorBufferHandle, NormalBufferHandle;

   // Information about mesh
   SVector3 Translation, Rotation, Scale;
   int TriangleCount;

   Player(SVector3* pos, SVector3* vel, CMesh * mod);
   ~Player();
   virtual void update(float dt);
   void draw();
   void collideWith(GameObject collided);
};
