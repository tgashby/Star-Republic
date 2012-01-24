#include "Bullet.h"


Bullet::Bullet(Coordiante loc, Coordiante vel, Angle dir, Angle angVel, GameObject& parent)
   : GameObject(loc, vel, dir, angVel)
{
   this->parent = parent;
}


Bullet::~Bullet(void)
{
}

void Bullet::doCollision( GameObject & other )
{
   if (other != parent)
   {
      // Injure the target!
   }
}

Model Bullet::getModel()
{
   throw std::exception("The method or operation is not implemented.");
}

BoundingObject Bullet::getBoundingObject()
{
   throw std::exception("The method or operation is not implemented.");
}

GameObject& Bullet::getParent()
{
   return parent;
}
