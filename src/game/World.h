#pragma once
#include <iostream>
#include <string>
#include "WorldPoint.h"
#include <vector>

using namespace std;

//Point in space vector for forward (Z),  up(Y), and side(X). Instead of Meshes, use object3ds
class World
{
public:
	World(const string fileName);
	~World();
	WorldPoint getCurrent();
	void setChoice(int next);
	WorldPoint getAt(int index);
	WorldPoint update(Vector3<float> playerPos);
	WorldPoint getPrevious();
        WorldPoint* getPreviousPointer();
        WorldPoint* getCurrentPointer();
	
private:
   WorldPoint parseLine(const string line);
   
   vector<WorldPoint> points;
   int currentPoint;
   //TODO
   int previousPoint;
};
