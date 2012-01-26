#include "Bullets.h"
#include <stdio.h>
#include <assert.h>

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
         i->nullify();
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

void Bullets::collideWith(Player* object)
{
   //fprintf(stderr, "\nCollidewithisbeingrun!");
   Collision* collider = new Collision();
   Bullet* temp = NULL;
   for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++) {
      //fprintf(stderr, "\nCollisioncheck is being run!\n");
      //Bullet* temp = &(*i);
      i->collisionCheck(object);
      if (i->gettoDie()) {
      //fprintf(stderr, "\nCollisioncheck doesn't crash!\n");
        object->collideWithBullet(10);
         //list.erase(i);
      }
   }
   delete(collider);
}

void Bullets::collideWith(Turrets* turrets)
{
   Turret* current = turrets->first;
   while (current != NULL) {
      for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++) {
         i->collisionCheck(current);
         if (i->gettoDie()) {
            current->collideWithBullet(i->damage);
            break;
         }
      }
      if (current->next == NULL) {
         break;
      }
      current = current->next;
   }
}

void Bullets::removeDead(SVector3 cameraPosition)
{
   for (std::list<Bullet>::iterator i = list.begin(); i != list.end(); i++) {
     if (i->getIgnore() == false) {
     //fprintf(stderr, "BRING OUT YER DEAD\n");
     if (i->gettoDie())
     {
         //fprintf(stderr, "HI DEAD\n");
         //list.erase(i);
         //i = list.begin();
         i->nullify();
     }
     }
   }
}
