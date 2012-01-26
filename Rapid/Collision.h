#pragma once

#include "GameObject.h"
#include <math.h>
#include "Util/SVector3.h"

class Collision{
public: 

   Collision();
   ~Collision();
   void collisionCheck(GameObject * first, GameObject * second);
};


