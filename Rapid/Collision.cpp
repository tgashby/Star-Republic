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

void Collision::collisionCheck(Player * first, Bullet * second) {

 /*  fprintf(stderr, "\nSTAGE1\n");
   float temp = first->size/2 + second->size/2;
   SVector3* a = first->getPosition();
   SVector3* b = second->getPosition();
   float aX = a->X;
fprintf(stderr, "\nSTAGEA\n");
   float aY = a->Y;
fprintf(stderr, "\nSTAGEB\n");
   float aZ = a->Z;
fprintf(stderr, "\nSTAGEC\n");
   float bX = b->X;
fprintf(stderr, "\nSTAGED\n");
   float bY = b->Y;
fprintf(stderr, "\nSTAGEE\n");
   float bZ = b->Z;
fprintf(stderr, "\nSTAGEF\n");
   //float x2 = x1->X;
   //float x2 = second->getPosition()->X;
   fprintf(stderr, "\nSTAGE2\n");
   if (first->getPosition() == NULL || second->getPosition() == NULL) fprintf(stderr, "\nNUUUUUUUUUULLS\n");
   if (typeid(GameObject) == typeid(*first))  fprintf(stderr, "\nfirst is a player\n");
   if (typeid(Bullet) == typeid(*second))  fprintf(stderr, "\nsecond is a bullet\n");
   float distance = sqrt((aX - bX) * (aX - bX) + (aY - bY) * (aY - bY) + (aZ - bZ) * (aZ - bZ));
   //float distance = 1;
   fprintf(stderr, "\nSTAGE3\n");
   if (distance < temp) {
   fprintf(stderr, "\nSTAGE4\n");
      first->collideWith((GameObject*)second);
   fprintf(stderr, "\nSTAGE5\n");
      second->collideWith(first);
   fprintf(stderr, "\nSTAGE6\n");
   //return 1;
   }
  // return 0;*/
}
