#pragma once
#include "Bullet.h"
#include "Map.h"
#include "Collision.h"
#include "GameObject.h"
#include "Player.h"
#include <list>

/*
#ifdef _WIN32
	#include <SDL.h>
	#include <SDL_opengl.h>
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_opengl.h>
#endif*/

class Bullets{
public:

   std::list<Bullet> list;

   Bullets();
   ~Bullets();
   void update(float dt, Map* map);
   void draw();
   void removeDead(SVector3 cameraPosition);
   void addBullet(SVector3* pos, SVector3* vel, CMesh* mod, float size, int damage);
   void collideWith(Player* object);
};
