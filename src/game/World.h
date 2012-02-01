#pragma once
#include <fileio>
#include <iostream>
#include <string>
#include "WorldPoint.h"
#include "Util/Vector.hpp"
#include <vector>

//Point in space vector for forward (Z),  up(Y), and side(X). Instead of Meshes, use object3ds
class World
{
public:
	// WorldPoint* points;
	// points = new WorldPoint[mysize];
	vector<WorldPoint> points;
        int currentPoint;
	

	World(const string fileName);
	~World();
	
};
