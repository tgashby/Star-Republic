#include "Collision.h"
#include "GameObject.h"

Collision::Collision() {
}

Collision::~Collision() {
}

void Collision::collisionCheck(GameObject * first, GameObject * second) {

   float temp = first->size/2 + second->size/2;
   float distance = sqrt((first->getPosition()->X - second->getPosition()->X) * (first->getPosition()->X - second->getPosition()->X) + (first->getPosition()->Y - second->getPosition()->Y) * (first->getPosition()->Y - second->getPosition()->Y) + (first->getPosition()->Z - second->getPosition()->Z) * (first->getPosition()->Z - second->getPosition()->Z));
   if (distance < temp) {
      first->collideWith(second);
      second->collideWith(first);
   //return 1;
   }
  // return 0;
}
