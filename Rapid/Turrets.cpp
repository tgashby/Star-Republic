#include "Turrets.h"

Turrets::Turrets(Map* map, int numToSpawn, Player* toAimAt)
{
   int i = 1;
   
   int random1 = rand() % (map->groundxmax - map->groundxmin);
   int random2 = rand() % (map->groundzmax - map->groundzmin);

   first = new Turret(random1, random2, toAimAt);
}
Turrets::~Turrets()
{

}
void Turrets::update(float dt)
{
   first->update(dt);
}
void Turrets::drawAll()
{
   first->draw();
}
void Turrets::shootIfPossible()
{

}
void Turrets::collideAllWith(Player p)
{

}
void Turrets::collideAllWith(Bullet b)
{

}
