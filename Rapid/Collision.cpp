#include "Collision.h"
#include "GameObject.h"
#include <stdio.h>
#include <typeinfo>
#include "Player.h"
#include "Bullet.h"

Collision::Collision() {
}

Collision::~Collision() {
}

void Collision::collisionCheck(GameObject * first, GameObject * second) {

   fprintf(stderr, "\nSTAGE1\n");
   float temp = first->size/2 + second->size/2;
   fprintf(stderr, "\nSTAGE2\n");
   if (first->getPosition() == NULL || second->getPosition() == NULL) fprintf(stderr, "\nNUUUUUUUUUULLS\n");
   if (typeid(GameObject) == typeid(*first))  fprintf(stderr, "\nfirst is a player\n");
   if (typeid(Bullet) == typeid(*second))  fprintf(stderr, "\nsecond is a bullet\n");
   float distance = sqrt((first->getPosition()->X - second->getPosition()->X) * (first->getPosition()->X - second->getPosition()->X) + (first->getPosition()->Y - second->getPosition()->Y) * (first->getPosition()->Y - second->getPosition()->Y) + (first->getPosition()->Z - second->getPosition()->Z) * (first->getPosition()->Z - second->getPosition()->Z));
   fprintf(stderr, "\nSTAGE3\n");
   if (distance < temp) {
   fprintf(stderr, "\nSTAGE4\n");
      first->collideWith(second);
   fprintf(stderr, "\nSTAGE5\n");
      second->collideWith(first);
   fprintf(stderr, "\nSTAGE6\n");
   //return 1;
   }
  // return 0;
}
