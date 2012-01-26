#pragma once
#include "GameObject.h"

#include <typeinfo>

#ifndef BASICS
#define BASICS

#include "CShader.h"
#include "CMeshLoader.h"

#endif

#include "Map.h"
#include "Turret.h"
#include "Bullet.h"
#include "Player.h"

class Turrets 
{
public:
   Turret* first;

   Turrets(Map* map, int numToSpawn, Player* toAimAt);
   ~Turrets();
   void update(float dt);
   void drawAll();
   void shootIfPossible();
   void collideAllWith(Player p);
   void collideAllWith(Bullet b);
};
