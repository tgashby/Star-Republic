#include "Turrets.h"
#include <stdio.h>

Turrets::Turrets(Map* map, int numToSpawn, Player* toAimAt)
{
   int i = 1;
   
   int random1 = rand() % (map->groundxmax - map->groundxmin);
   int random2 = rand() % (map->groundzmax - map->groundzmin);

   first = new Turret(random1, random2, toAimAt);
   //printf("Created at %d x, %d z\n", random1, random2);
   
   Turret* current = first;
   while (i < numToSpawn)
   {
      random1 = rand() % (map->groundxmax - map->groundxmin);
      random2 = rand() % (map->groundzmax - map->groundzmin);
      //printf("Created at %d x, %d z\n", random1, random2);

      Turret* temp = new Turret(random1, random2, toAimAt);
      temp->next = 0;
      current->next = temp;
      current = temp;

      i++;
   }
}
Turrets::~Turrets()
{

}
void Turrets::update(float dt)
{
   Turret* current = first;
   Turret* prev = 0;
   while(current != 0)
   {
      current->update(dt);

      if (current->health <= 0)
      {
         if (prev == 0)
         {
            first = current->next;
         }
         else
         {
            prev->next = current->next;
         }
      }
      
      prev = current;
      current = current->next;
   }
}
void Turrets::drawAll()
{
   Turret* current = first;
   while(current != 0)
   {
      if (current->health > 0) 
         current->draw();
      current = current->next;
   }
}
void Turrets::shootIfPossible()
{
   Turret* current = first;
   while(current != NULL)
   {
      if (current->health > 0)
         current->tryToShoot();
      if (current->next == NULL) {
         break;
      }
      else {
      current = current->next;
      }
   }
}
void Turrets::collideAllWith(Player* p)
{
   Turret* current = first;
   while(current != 0)
   {
      if (current->health > 0)
         current->collideWith(p);
      current = current->next;
   }
}
void Turrets::collideAllWith(Bullet b)
{

}
