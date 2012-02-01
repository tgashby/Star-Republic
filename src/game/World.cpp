#include "World.h"
#include <iostream>
#include <fstream>

World::World(const string fileName)
{
   string line;
   streampos linestart;
   ifstream infile(fileName.c_str());
   
  // vec3 *v;
   //vec2 *t;;

   if(infile.is_open())
   {
      while(!infile.eof())
      {
         linestart = infile.tellg();
         getline(infile,line);
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
   
   int tempInt, tempLeft, tempMid, tempRight;

   sscanf(line.c_str(), "%d", &tempInt);
   
   sscanf(line.c_str(), "%f %f %f %f %f %f %f %f %f %f %f %f", 
          &tempPosition.x, &tempPosition.y, &tempPosition.z, &tempUp.x, 
          &tempUp.y, &tempUp.z, &tempForward.x, &tempForward.y, 
          &tempForward.z, &tempSide.x, &tempSide.y, &tempSide.z);
   
   WorldPoint point (tempPosition, tempUp, tempForward, tempSide);
   
   tempInt = sscanf(line.c_str(), "%d %d %d", &tempLeft, &tempMid, &tempRight);
   
   if (tempInt >= 1) {
      point.setLeftID(tempLeft);
   }
   
   if (tempInt >= 2) {
      point.setMiddleID(tempMid);
   }
   
   if (tempInt == 3) {
      point.setRightID(tempRight);
   }
   
   point.setNumberOfIDs(tempInt);
   
   return point;
}

World::~World()
{
}
