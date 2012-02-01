#include "WorldPoint.h"

WorldPoint::WorldPoint(Vector3<float> position, Vector3<float> up, Vector3<float> forward, Vector3<float> side)
{
   this->position = position;
   this->up = up;
   this->forward = forward;
   this->side = side;
}

WorldPoint::~WorldPoint()
{
}

void WorldPoint::setPosition(float x, float y, float z)
{
   position.x = x;
   position.y = y;
   position.z = z;
}

void WorldPoint::setUp(float x, float y, float z)
{
   up.x = x;
   up.y = y;
   up.z = z;
}

void WorldPoint::setForward(float x, float y, float z)
{
   forward.x = x;
   forward.y = y;
   forward.z = z;
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

int WorldPoint::getNumberOfIDs()
{
  return numberOfIDs;
}

int WorldPoint::getLeftID()
{
  return leftID;
}

int WorldPoint::getMiddleID()
{
  return middleID;
}

int WorldPoint::getRightID()
{
   return rightID;
}
