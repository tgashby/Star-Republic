#include "PathPoint.h"

PathPoint::PathPoint(Vector3<float> position, Vector3<float> up, Vector3<float> forward, Vector3<float> side)
{
   this->position = position;
   this->up = up.Normalized();
   this->forward = forward.Normalized();
   this->side = side.Normalized();
}

PathPoint::PathPoint(vec3 position, vec3 forward, vec3 up, ivec4 links) {
  this->position = position;
  this->up = up.Normalized();
  this->forward = forward.Normalized();
  setFirstID(links.y);
   setNumberOfIDs(1);
  if (links.z != -1) {
     setNumberOfIDs(2);
    setSecondID(links.z);
  }
  if (links.w != -1) {
     setNumberOfIDs(3);
    setThirdID(links.w);
  }
}

PathPoint::~PathPoint()
{
}

void PathPoint::setPosition(float x, float y, float z)
{
   position.x = x;
   position.y = y;
   position.z = z;
}

void PathPoint::setUp(float x, float y, float z)
{
   up.x = x;
   up.y = y;
   up.z = z;
}

void PathPoint::setForward(float x, float y, float z)
{
   forward.x = x;
   forward.y = y;
   forward.z = z;
}

void PathPoint::setFirstID(int ID)
{
   firstID = ID;
}

void PathPoint::setSecondID(int ID)
{
   secondID = ID;
}

void PathPoint::setThirdID(int ID)
{
   thirdID = ID;
}

void PathPoint::setNumberOfIDs(int number)
{
   numberOfIDs = number;
}

int PathPoint::getNumberOfIDs()
{
  return numberOfIDs;
}

int PathPoint::getFirstID()
{
  return firstID;
}

int PathPoint::getSecondID()
{
  return secondID;
}

int PathPoint::getThirdID()
{
   return thirdID;
}

Vector3<float> PathPoint::getPosition() 
{
  return position;
}

Vector3<float> PathPoint::getUp() 
{
  return up;
}

Vector3<float> PathPoint::getForward()
{
  return forward;
}

Vector3<float> PathPoint::getSide()
{
  return side;
}
