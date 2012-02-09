#include "World.h"
#include <iostream>
#include <fstream>
#define RANGE_CHECK 50
#define MID_BUFFER_WIDTH 2
#include <cmath>

World::World(const string fileName)
{
   string line;
   streampos linestart;
   ifstream infile(fileName.c_str());
   currentPoint = 1;
   previousPoint = 0;

   if(infile.is_open())
   {
     while(getline(infile, line))
       {
         points.push_back(parseLine(line));
      }
   }
   else
   { cerr << "Problem is in World: the infile was never opened. \n"; }
}

World::World(const string fileName, Modules* m_modules)
{
   currentPoint = 1;
   previousPoint = 0;

  WorldData *worldData = m_modules->resourceManager->
    readWorldData("maps/world2.wf");
  
  for (int i = 0; i < worldData->links.size(); i++) {
    cerr << "i is " << i << "\n";
    points.push_back(WorldPoint(worldData->path[i*3], worldData->path[i*3 + 1],
				worldData->path[i*3 + 2], worldData->links[i]));
  }
  currentPoint = 1;
  previousPoint = 0;
  cerr << points.size();
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
		       &tempPosition.z, &tempForward.x, &tempForward.y, &tempForward.z, 
		       &tempUp.x, &tempUp.y, &tempUp.z, 
		       &tempSide.x, &tempSide.y, &tempSide.z, &tempLeft,
		       &tempMid, &tempRight) - 13;

   cerr << "X IS: " << tempPosition.x << ", Y IS: " << tempPosition.y << ", Z IS: " << tempPosition.z << "\n";
   
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
WorldPoint* World::getCurrentPointer() {
   return &points.at(currentPoint);
}
WorldPoint World::getPrevious() {
  return points.at(previousPoint);
}
WorldPoint* World::getPreviousPointer() {
  return &points.at(previousPoint);
}

void World::setChoice(int next) {
  previousPoint = currentPoint;
  currentPoint = next;
}

World::~World()
{

}

WorldPoint World::getAt(int index) 
{
   //cout << "index: " << index << "\n";
   //cout << "from vector size: " << points.size() << "\n";
  return points.at(index);
}

WorldPoint World::update(Vector3<float> playerPos)
{
  float D_val;
  WorldPoint current = getCurrent();
  WorldPoint previous = getPrevious();
  float diffX = playerPos.x - current.getPosition().x;
  float diffY = playerPos.y - current.getPosition().y;
  float diffZ = playerPos.z - current.getPosition().z;
  float playerDistFromPlane = 0;
  float firstDistFromPlane = 0;
  Vector3<float> vect1 (current.getPosition().x - previous.getPosition().x,
			current.getPosition().y - previous.getPosition().y,
			current.getPosition().z - previous.getPosition().z);
   cout << "it worked1\n";
  Vector3<float> vect2 (current.getPosition().x + current.getUp().x,
			current.getPosition().y + current.getUp().y,
			current.getPosition().z + current.getUp().z);
   cout << "it worked2\n";
  Vector3<float> normal;
  float distance = sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
  WorldPoint firstChoice = getAt(current.getFirstID());

  if (distance < RANGE_CHECK) {
    if (current.getNumberOfIDs() == 1) {
      setChoice(current.getFirstID());
      return getCurrent();
    }
    normal = vect1.Cross(vect2);
    D_val = (current.getPosition().x * normal.x + current.getPosition().y 
	     * normal.y * current.getPosition().z * normal.z) * -1.0f;
    
    playerDistFromPlane = current.getPosition().x * playerPos.x + 
      current.getPosition().y * playerPos.y + current.getPosition().z 
      * playerPos.z + D_val;
    
    if (abs(playerDistFromPlane) < MID_BUFFER_WIDTH && 
	current.getNumberOfIDs() == 3) {
      setChoice(current.getSecondID());
      return getCurrent();
    }
    
    firstDistFromPlane = current.getPosition().x * firstChoice.getPosition().x 
      + current.getPosition().y * firstChoice.getPosition().y + 
      current.getPosition().z * firstChoice.getPosition().z + D_val;

    if (playerDistFromPlane / abs(playerDistFromPlane) == 
	firstDistFromPlane / abs(firstDistFromPlane)) {
      setChoice(current.getFirstID());
      return getCurrent();
    }
    else {
      setChoice(current.getThirdID());
      return getCurrent();
    }
  }
  return getCurrent();
}

