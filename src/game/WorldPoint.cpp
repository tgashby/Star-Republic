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

void WorldPoint::setFirstID(int ID)
{
   firstID = ID;
}

void WorldPoint::setSecondID(int ID)
{
   secondID = ID;
}

void WorldPoint::setThirdID(int ID)
{
   thirdID = ID;
}

void WorldPoint::setNumberOfIDs(int number)
{
   numberOfIDs = number;
}

int WorldPoint::getNumberOfIDs()
{
  return numberOfIDs;
}

int WorldPoint::getFirstID()
{
  return firstID;
}

int WorldPoint::getSecondID()
{
  return secondID;
}

int WorldPoint::getThirdID()
{
   return thirdID;
}

Vector3<float> WorldPoint::getPosition() 
{
  return position;
}

Vector3<float> WorldPoint::getUp() 
{
  return up;
}

Vector3<float> WorldPoint::getForward()
{
  return forward;
}

Vector3<float> WorldPoint::getSide()
{
  return side;
}
