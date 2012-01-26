#include "Turret.h"
#include <stdio.h>

Turret::Turret(int xloc, int zloc, Player* toAimAt)
{
   health = 100;
   Translation.X = xloc;
   Translation.Y = 0;
   Translation.Z = zloc;

   aim = toAimAt;
   
   Rotation.X = 0;
   Rotation.Y = 0;
   Rotation.Z = 0;
}
Turret::~Turret()
{

}
void Turret::update(float dt)
{
   SVector3* d = new SVector3(0,0,0);
   d->X = Translation.X - aim->getPosition()->X;
   d->Y = Translation.Y - aim->getPosition()->Y;
   d->Z = Translation.Z - aim->getPosition()->Z;
   
   d->X /= d->length();
   d->Y /= d->length();
   d->Z /= d->length();

   Rotation.X = (-atan(sqrt(d->X * d->X + d->Z * d->Z) / d->Y) * 180 / 3.1415926); 
   Rotation.Y = (-atan(d->X / d->Z) * 180 / 3.1415926); 
   //Rotation.Z = (cos(d->Z) * 180 / 3.1415926); 
}
void Turret::draw()
{
   if (health > 0)
   {
      glPushMatrix();

      glTranslatef(Translation.X, Translation.Y, Translation.Z);
      glScalef(2, 0.5, 2);
      glutSolidCube(1);
      glScalef(0.5, 2, 0.5);
      glTranslatef(0, 0.5, 0);
      glRotatef(Rotation.Y, 0, 1, 0);
      glScalef(1.5, 0.5, 1.5);
      glutSolidCube(1);
      glScalef(0.66, 2, 0.66);
      glRotatef(Rotation.X, 1, 0, 0);
      glScalef(0.5, 4, 0.5);
      glutSolidCube(1);        

      glPopMatrix();
   }
}
void Turret::tryToShoot()
{

}
float Turret::getSize()
{
   return 1.5;
}
void Turret::collideWith(Player* p)
{
   if (p->getTranslation()->Y < -0.8)
   { 
      float x = p->getTranslation()->X + 6;
      if (x - p->getSize() < Translation.X + getSize()  && x + p->getSize() > Translation.X - getSize())
      {
         float z = p->getTranslation()->Z + 3;
         if (z - p->getSize() < Translation.Z + getSize()  && z + p->getSize() > Translation.Z - getSize())
         {
            printf("hit: player - %f, %f, %f  ;  turret - %f, %f, %f\n", p->getTranslation()->X, p->getTranslation()->Y, p->getTranslation()->Z, Translation.X, Translation.Y, Translation.Z);
            p->health -= 20;
            this->health = 0;
         }
      }
   }
}
void Turret::collideWith(Bullet b)
{

}
