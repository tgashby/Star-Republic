#pragma once
#include "GameObject.h"

#include <typeinfo>

#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"

#endif
#include "Bullet.h"
#include "Player.h"
#include <math.h>

class Turret
{
public:
   Turret* next;
   Player* aim;
   int health, cooldown;
   CShader* shade;

   // Handles for VBOs
   GLuint PositionBufferHandle, ColorBufferHandle, NormalBufferHandle;

   // Information about mesh
   SVector3 Translation, Rotation, Scale;
   int TriangleCount;

   Turret(int xloc, int yloc, Player* toAimAt);
   ~Turret();
   void update(float dt);
   void draw();
   float getSize();
   void tryToShoot();
   void collideWith(Player* p);
   void collideWith(Bullet b);
};
