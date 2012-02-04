#include "World.h"
#include <iostream>
#include <fstream>
#define RANGE_CHECK 4
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
  Vector3<float> vect2 (current.getPosition().x + current.getUp().x,
			current.getPosition().y + current.getUp().y,
			current.getPosition().z + current.getUp().z);
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
	     * normal.y * current.getPosition().z * normal.z) * -1.0;
    
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
