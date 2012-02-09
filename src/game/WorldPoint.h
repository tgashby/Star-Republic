#pragma once
#include "../engine/Interfaces.h"

class WorldPoint
{
public:
   
WorldPoint(Vector3<float> position, Vector3<float> up, Vector3<float> forward, Vector3<float> side);
 WorldPoint(vec3 position, vec3 forward, vec3 up, ivec4 links);
~WorldPoint();
void setPosition(float x, float y, float z);
void setUp(float x, float y, float z);
void setForward(float x, float y, float z);
void setSide(float x, float y, float z);
void setFirstID(int id);
void setSecondID(int id);
void setThirdID(int id);
void setNumberOfIDs(int number);
 int getNumberOfIDs();
 int getFirstID();
 int getSecondID();
 int getThirdID();
 Vector3<float> getPosition();
 Vector3<float> getUp();
 Vector3<float> getForward();
 Vector3<float> getSide();

private:
   Vector3<float> position;
   Vector3<float> up;
   Vector3<float> forward;
   Vector3<float> side;
   int firstID, secondID, thirdID;
   int numberOfIDs;  //Keeps track of the number of IDs linking from this point. The convention is that left is filled first, then middle, then right.
   //Example: if numberOfIDs == 2, then leftID and middleID have indexes to points, and rightID would not.
};

