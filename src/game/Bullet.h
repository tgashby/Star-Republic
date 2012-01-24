#pragma once

#include <typeinfo>
#include "GameObject.h"

/**
 * Bullet Class, represents a bullet shot from anything                                                                 
 */
class Bullet :
   public GameObject
{
public:
   Bullet(Coordiante loc, Coordiante vel, Angle dir, Angle angVel, GameObject& parent, BoundingObject bounds);
   ~Bullet(void);

   virtual void doCollision( GameObject & other );

   virtual Model getModel();

   virtual BoundingObject getBoundingObject();

   /**
    * Returns the Bullet's parent, useful in collision detection                                                                 
    */
   GameObject& getParent();

private:
   GameObject& parent;
};
