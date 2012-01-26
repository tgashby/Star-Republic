#include "Bullets.h"
#include "Map.h"
#include "GameObject.h"
#include "Bullet.h"
#include <stdio.h>

Bullets::Bullets()
{
}

Bullets::~Bullets()
{
}

void Bullets::update(float dt, Map* map)
{
   for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++)
   {
   //printf("\nHalfway through update.");
     if (i->gettoDie())
     {
         list.erase(i);
     }
   }

   for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++) {
      i->update(dt);
   //printf("\nNear end through update.");
      /*if (i->getPosition()->X < map->xmin || i->getPosition()->X > map->xmax
         || i->getPosition()->Y < map->ymin || i->getPosition()->Y > map->ymax) {
         list.erase(i);
      }*/
   }

}

void Bullets::draw()
{
   for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++) {
      i->draw();
   }
}

void Bullets::addBullet(SVector3* pos, SVector3* vel, CMesh* mod, float size, int damage)
{
   list.push_back(Bullet(pos, vel, mod, size, damage));
}

void Bullets::collideWith(GameObject* object)
{
   //fprintf(stderr, "\nCollidewithisbeingrun!");
   Collision* collider = new Collision();
   for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++) {
      fprintf(stderr, "\nCollisioncheck is being run!\n");
      collider->collisionCheck(object, (GameObject*)&i);
      fprintf(stderr, "\nCollisioncheck doesn't crash!\n");
         //object->colideWith((GameObject*)&i);
         //list.erase(i);
     // }
   }
   delete(collider);
}

void Bullets::removeDead(SVector3 cameraPosition)
{
   for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++) {
     if (i->gettoDie() || i->getPosition()->Z < cameraPosition.Z)
     {
         list.erase(i);
     }
   }
}
