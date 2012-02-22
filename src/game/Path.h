#ifndef StarRepub_Path_h
#define StarRepub_Path_h

#include <iostream>
#include <string>
#include "PathPoint.h"
#include "Interfaces.h"
#include <vector>

using namespace std;

//Point in space vector for forward (Z),  up(Y), and side(X). Instead of Meshes, use object3ds
class Path {
public:
   Path(WorldData *data);
   //Path(const string fileName);
   //Path(const string fileName, Modules* m_modules);
   ~Path();
   PathPoint getCurrent();
   void setChoice(int next);
   PathPoint getAt(int index);
   PathPoint update(Vector3<float> playerPos);
   PathPoint getPrevious();
   PathPoint* getPreviousPointer();
   PathPoint* getCurrentPointer();
   int getSize();
   WorldData* worldData;
   
 private:
  PathPoint parseLine(const string line);
   
  vector<PathPoint> points;
  int currentPoint;
  //TODO
  int previousPoint;
};

#endif
