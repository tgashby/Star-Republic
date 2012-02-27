#include "PathPoint.h"

PathPoint::PathPoint(PathPointData pointData) {
   //PathPoint(pointData.loc, pointData.fwd, pointData.up, pointData.links);
   position = pointData.loc;
   up = pointData.up;
   forward = pointData.fwd;
   ivec4 links = pointData.links;
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

PathPoint::PathPoint(vec3 position, vec3 up, vec3 forward, vec3 side) {
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

PathPoint::~PathPoint() {
}

void PathPoint::setPosition(float x, float y, float z) {
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

vec3 PathPoint::getPosition() 
{
  return position;
}

vec3 PathPoint::getUp() 
{
  return up;
}

vec3 PathPoint::getForward()
{
  return forward;
}

vec3 PathPoint::getSide()
{
  return side;
}
