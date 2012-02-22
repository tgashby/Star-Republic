#include "Path.h"
#include <iostream>
#include <fstream>
#define RANGE_CHECK 50
#define MID_BUFFER_WIDTH 2
#include <cmath>

Path::Path(WorldData *data) {
   worldData = data;
   currentPoint = 1;
   previousPoint = 0;
   
   vector<PathPointData>::iterator newPoint;
   for (newPoint = data->path.begin(); newPoint != data->path.end(); ++newPoint) {
      PathPoint point = PathPoint(*newPoint);
      points.push_back(point);
   }
   
   currentPoint = 1;
   previousPoint = 0;
}
/*
Path::Path(const string fileName)
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
   { cerr << "Problem is in Path: the infile was never opened. \n"; }
}

Path::Path(const string fileName, Modules* m_modules)
{
  currentPoint = 1;
  previousPoint = 0;

  worldData = m_modules->resourceManager->
  readWorldData(fileName);
  
  for (int i = 0; i < worldData->links.size(); i++) {
//    cerr << "i is " << i << "\n";
    points.push_back(PathPoint(worldData->path[i*3], worldData->path[i*3 + 1],
				worldData->path[i*3 + 2], worldData->links[i]));
  }
   
  currentPoint = 1;
  previousPoint = 0;
//  cerr << points.size();
}

PathPoint Path::parseLine(const string line)
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

//   cerr << "X IS: " << tempPosition.x << ", Y IS: " << tempPosition.y << ", Z IS: " << tempPosition.z << "\n";
   
   PathPoint point (tempPosition, tempUp, tempForward, tempSide);
   
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
}*/

PathPoint Path::getCurrent() {
  return points.at(currentPoint);
}

PathPoint* Path::getCurrentPointer() {
   return &points.at(currentPoint);
}

PathPoint Path::getPrevious() {
  return points.at(previousPoint);
}

PathPoint* Path::getPreviousPointer() {
  return &points.at(previousPoint);
}

void Path::setChoice(int next) {
  previousPoint = currentPoint;
  currentPoint = next;
}

Path::~Path()
{

}

PathPoint Path::getAt(int index) 
{
  return points.at(index);
}

PathPoint Path::update(Vector3<float> refPos, Vector3<float> playerPos)
{
  float D_val;
  PathPoint current = getCurrent();
  PathPoint previous = getPrevious();
  float diffX = refPos.x - current.getPosition().x;
  float diffY = refPos.y - current.getPosition().y;
  float diffZ = refPos.z - current.getPosition().z;
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
  PathPoint firstChoice = getAt(current.getFirstID());

  if (distance < RANGE_CHECK) {
    if (current.getNumberOfIDs() == 1) {

      setChoice(current.getFirstID());
      return getCurrent();
    }

    normal = vect1.Cross(vect2).Normalized();
    D_val = (current.getPosition().x * normal.x + current.getPosition().y 
	     * normal.y + current.getPosition().z * normal.z) * -1.0f;
    
    playerDistFromPlane = (normal.x * playerPos.x) + 
       (normal.y * playerPos.y) + (normal.z * playerPos.z) + D_val;
    
    if (abs(playerDistFromPlane) < MID_BUFFER_WIDTH && 
	current.getNumberOfIDs() == 3) {
      setChoice(current.getThirdID());
      return getCurrent();
    }
    
    firstDistFromPlane = normal.x * firstChoice.getPosition().x 
       + normal.y * firstChoice.getPosition().y 
       + normal.z * firstChoice.getPosition().z + D_val;

    if (playerDistFromPlane / abs(playerDistFromPlane) == 
	firstDistFromPlane / abs(firstDistFromPlane)) {
      setChoice(current.getFirstID());
      return getCurrent();
    }
    else {
      setChoice(current.getSecondID());
      return getCurrent();
    }
  }
  return getCurrent();
}

int Path::getSize() {
  return points.size();
}
