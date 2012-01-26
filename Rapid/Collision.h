#pragma once

#include "GameObject.h"
#include <math.h>
#include "Bullet.h"
#include "Util/SVector3.h"
#include "Player.h"

class Collision{
public: 

   Collision();
   ~Collision();
   void collisionCheck(Player * first, Bullet * second);
};


