#include "World.h"
#include <iostream>
#include <fstream>

World::World(const string fileName)
{
   string line;
   streampos linestart;
   ifstream infile(fileName.c_str());
   currentPoint = 0;

   if(infile.is_open())
   {
     while(getline(infile, line))
       {
         points.push_back(parseLine(line));
      }
   }
}

WorldPoint World::parseLine(const string line)
{
   Vector3<float> tempPosition;
   Vector3<float> tempUp;
   Vector3<float> tempForward;
   Vector3<float> tempSide;
   
   int tempInt, tempLeft, tempMid, tempRight, totalPaths;
   
   totalPaths = sscanf(line.c_str(), 
		       "%d %f %f %f %f %f %f %f %f %f %f %f %f %d %d %d", 
		       &tempInt, &tempPosition.x, &tempPosition.y, 
		       &tempPosition.z, &tempUp.x, &tempUp.y, &tempUp.z, 
		       &tempForward.x, &tempForward.y, &tempForward.z, 
		       &tempSide.x, &tempSide.y, &tempSide.z, &tempLeft,
		       &tempMid, &tempRight) - 13;
   
   WorldPoint point (tempPosition, tempUp, tempForward, tempSide);
   
   if (totalPaths >= 1) {
      point.setFirstID(tempLeft);
   }
   
   if (totalPaths >= 2) {
      point.setSecondID(tempMid);
   }
   
   if (totalPaths == 3) {
      point.setThirdID(tempRight);
   }
   
   point.setNumberOfIDs(totalPaths);
   
   return point;
}

WorldPoint World::getCurrent() {
  return points.at(currentPoint);
}

void World::setChoice(int next) {
  currentPoint = next;
}

World::~World()
{

}

WorldPoint World::getAt(int index) 
{
  return points.at(index);
}
