#pragma once
#include "../engine/Vector.hpp"

class WorldPoint
{
public:
   
WorldPoint(Vector3<float> position, Vector3<float> up, Vector3<float> forward, Vector3<float> side);
~WorldPoint();
void setPosition(float x, float y, float z);
void setUp(float x, float y, float z);
void setForward(float x, float y, float z);
void setSide(float x, float y, float z);
void setLeftID(int id);
void setMiddleID(int id);
void setRightID(int id);
void setNumberOfIDs(int number);
 int getNumberOfIDs();
 int getLeftID();
 int getMiddleID();
 int getRightID();

private:
   Vector3<float> position;
   Vector3<float> up;
   Vector3<float> forward;
   Vector3<float> side;
   int leftID, middleID, rightID;
   int numberOfIDs;  //Keeps track of the number of IDs linking from this point. The convention is that left is filled first, then middle, then right.
   //Example: if numberOfIDs == 2, then leftID and middleID have indexes to points, and rightID would not.
};

