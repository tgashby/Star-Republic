#include "WorldPoint.h"

WorldPoint::WorldPoint()
{
}

WorldPoint::~WorldPoint()
{
}

void WorldPoint::setPosition(float x, float y, float z)
{
   position->x = x;
   position->y = y;
   position->z = z;
}

void WorldPoint::setUp(float x, float y, float z)
{
   up->x = x;
   up->y = y;
   up->z = z;
}

void WorldPoint::setForward(float x, float y, float z)
{
   forward->x = x;
   forward->y = y;
   forward->z = z;
}

void WorldPoint::setLeftID(int ID)
{
   leftID = ID;
}

void WorldPoint::setMiddleID(int ID)
{
   middleID = ID;
}

void WorldPoint::setRightID(int ID)
{
   rightID = ID;
}

void WorldPoint::setNumberOfIDs(int number)
{
   numberOfIDs = number;
}
