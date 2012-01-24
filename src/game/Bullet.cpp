#include "Bullet.h"


Bullet::Bullet(Coordiante loc, Coordiante vel, Angle dir, Angle angVel, GameObject& parent, BoundingCylinder bounds)
   : GameObject(loc, vel, dir, angVel, bounds)
{
   this->parent = parent;
   stillAlive = true;
}


Bullet::~Bullet(void)
{
}

void Bullet::doCollision( GameObject & other )
{
   if (stillAlive && other != parent)
   {
      // Injure the target!
      other.takeDamage();
      
      // Register that you are going away to your parent
      stillAlive = false;
   }
}

Model Bullet::getModel()
{
   GameObject::getModel();
}

BoundingObject Bullet::getBoundingObject()
{
   GameObject::getBoundingObject();
}

GameObject& Bullet::getParent()
{
   return parent;
}

bool Bullet::isAlive()
{
   return stillAlive;
}