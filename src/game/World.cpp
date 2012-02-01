#include "World.h"
#include <iostream>
#include <fstream>

World::World(const string fileName)
{
   string line;
   streampos linestart;
   ifstream infile(fileName.c_str());
   float tempX, float tempY, float tempZ;
   WorldPoint *wp;
  // vec3 *v;
   //vec2 *t;;

   if(infile.is_open())
   {
      while(!infile.eof())
      {
         long start;
         
         linestart = infile.tellg();
         getline(infile,line)
         wp = parseLine(line);
         points.push_back(wp);
      }
   }
}

WorldPoint World::parseLine(const string line)
{
   vector3<float> tempPosition;
   vector3<float> tempUp;
   vector3<float> tempForward;
   vector3<float> tempSide;
   int tempInt, tempLeft, tempMid, tempRight;

         sscanf(line.c_str(), "%d", &tempInt);
         sscanf(line.c_str(), "%f %f %f %f %f %f %f %f %f %f %f %f", &tempPosition.x,
          &tempPosition.y, &tempPosition.z, &tempUp.x, &tempUp.y, &tempUp.z,
          &tempForward.x, &tempForward.y, &tempForward.z, &tempSide.x,
          &tempSide.y, &tempSide.z);
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
         
         
         
         
     
   

         
         
         
         
         

	
}

World::~World()
{
}
