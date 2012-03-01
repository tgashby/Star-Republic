#ifndef StarRepub_PathPoint_h
#define StarRepub_PathPoint_h

#include "../engine/Interfaces.h"
#include "GameObject.h"
#include "../engine/Object3d.h"

struct Quadrant
{
   /**
    * Quadrant constructor
    * @param startPt the first point
    * @param endPt the second point
    */

  Quadrant (PathPointData point)
  : m_Point(point), m_bounds(SPHERE_RADIUS)
  {
    
  }
   
   PathPointData m_Point;
   std::list<GameObject*> m_gameObjects;
   std::list<IObject3d*> m_obj3Ds;
   
   /**
    * BoundingSphere representing the space around a single point we wish to draw.
    * Used in calculating whether an object is in the quadrant or not.
    */
   BoundingSphere m_bounds;
   
   static const float SPHERE_RADIUS;
};

class PathPoint {
 public:
   PathPoint(PathPointData pointData);
   PathPoint(vec3 position, vec3 up, vec3 forward, vec3 side);
   /*  PathPoint(vec3 position, vec3 forward, vec3 up, ivec4 links);*/
   ~PathPoint();
   void setPosition(float x, float y, float z);
   void setUp(float x, float y, float z);
   void setForward(float x, float y, float z);
   void setSide(float x, float y, float z);
   void setFirstID(int id);
   void setSecondID(int id);
   void setThirdID(int id);
   void setNumberOfIDs(int number);
   int getNumberOfIDs();
   int getFirstID();
   int getSecondID();
   int getThirdID();
   vec3 getPosition();
   vec3 getUp();
   vec3 getForward();
   vec3 getSide();

   Quadrant getQuadrant();
   bool fitsQuadrant(vec3 gameObjPos);
   void addToQuadrant(GameObject* gameObj, Object3d* obj3D);

private:
   vec3 position;
   vec3 up;
   vec3 forward;
   vec3 side;

   int firstID, secondID, thirdID;   
   int numberOfIDs;  
   //Keeps track of the number of IDs linking from this point. The convention is that left is filled first, then middle, then right.
   //Example: if numberOfIDs == 2, then leftID and middleID have indexes to points, and rightID would not.

   Quadrant m_quadrant;
};

#endif
