#pragma once
#include "vector.hpp"

class WorldPoint
{
public:

vector3<float> position;
vector3<float> up;
vector3<float> forward;
vector3<float> side;
int leftID, middleID, rightID;
int numberOfIDs;  //Keeps track of the number of IDs linking from this point. The convention is that left is filled first, then middle, then right.
                  //Example: if numberOfIDs == 2, then leftID and middleID have indexes to points, and rightID would not.

WorldPoint(vector3<float> position, vector3<float> up, vector3<float> forward, vector3<float> side);
~WorldPoint();
void setPosition(float x, float y, float z);
void setUp(float x, float y, float z);
void setForward(float x, float y, float z);
void setSide(float x, float y, float z);
void setLeftID(int id);
void setMiddleID(int id);
void setRightID(int id);
void setNumberOfIDs(int number);
};

