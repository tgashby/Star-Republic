#include "Bullets.h"
#include "Map.h"
#include "GameObject.h"
#include "Bullet.h"

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
     if (i->gettoDie())
     {
         list.erase(i);
     }
   }

   for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++) {
      i->update(dt);
      if (i->getPosition()->X < map->xmin || i->getPosition()->X > map->xmax
         || i->getPosition()->Y < map->ymin || i->getPosition()->Y > map->ymax) {
         list.erase(i);
      }
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
 /*  for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++) {
      if (Collision::collisionCheck(object, (GameObject*)&i)) {
         object->collideWith((GameObject*)&i);
         i->~Bullet();
         list.erase(i);
      }
   } */
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
