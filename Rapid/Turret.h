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
   CMesh *mod;

   // Handles for VBOs
   GLuint PositionBufferHandle1, ColorBufferHandle1, NormalBufferHandle1;
   GLuint PositionBufferHandle2, ColorBufferHandle2, NormalBufferHandle2;
   GLuint PositionBufferHandle3, ColorBufferHandle3, NormalBufferHandle3;

   // Information about mesh
   SVector3 Translation, Rotation, Scale;
   int TriangleCount;

   Turret(int xloc, int yloc, Player* toAimAt);
   ~Turret();
   void update(float dt);
   void draw();
   void tryToShoot();
   void collideWith(Player p);
   void collideWith(Bullet b);
};
