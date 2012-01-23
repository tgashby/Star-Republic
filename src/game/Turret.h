//
//  Turret.h
//  476 Game
//
//  Created by Taggart Ashby on 1/22/12.
//  Copyright 2012 476 Proj
//

#ifndef _76_Game_Turret_h
#define _76_Game_Turret_h

#include <list>
#include <typeinfo>
#include "../engine/ProjIncludes.h"
#include "../engine/Vector.hpp"
#include "GameObject.h"
#include "Player.h"

/**
 * Turret Class
 * Represents a turret game object
 */
class Turret : GameObject
{
public:
   Turret(Vector3<float> position, Vector3<float> direction, 
	   float velocity, BoundingStructure bounds, BoundingStructre range, Player& player);

   virtual void tic(int dt);

   virtual void draw();

   virtual void doCollision( GameObject & other );

private:
   /**
    * Check range determines if the player is within the turret's sight range.                                                                 
    */
   bool checkRange();

   /**
    * Fire shoots a single bullet at the player's current position                                                                 
    */
   void fire();

   virtual Model getModel();

   virtual BoundingObject getBoundingObject();

   Player& player;
   BoundingStructure range;
   std::list<Bullet> bullets;
   bool firing;
};

#endif
