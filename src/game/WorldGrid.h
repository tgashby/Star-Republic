//
//  WorldGrid.h
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#ifndef StarRepublic_WorldGrid_h
#define StarRepublic_WorldGrid_h

#include <list>
#include "GameObject.h"

struct Quadrant
{
   Quadrant(vec3 startPt, vec3 endPt) 
   {
      m_startPt = startPt;
      m_endPt = endPt;
   }
   
   std::list<GameObject*> m_gameObjects;
   vec3 m_startPt;
   vec3 m_endPt;
};

/**
 * WorldGrid represents the world as a uniform spatial subdivision.
 * It is a list of cubes encompassing the path and all it's objects.
 */
class WorldGrid 
{
public:
   /**
    * Constructs a WorldGrid with the given side length.
    * This will be used for all dimensions of the grid.
    */
   WorldGrid(WorldData& world);
   
   /**
    * Frees the grid in memory.
    */
   ~WorldGrid();
   
   /**
    * 
    */
   std::list<GameObject*> checkGrid(float x, float y, float z);
   
   /**
    * 
    */
   void tic(uint64_t dt);
   
   /**
    * 
    */
   std::list<GameObject*> getUpdateableObjects();
   
   /**
    * 
    */
   void checkCollisions();
   
private:
   WorldData& m_world;
   std::vector<Quadrant> m_quadrants;
   
   void makeGrid();
};

#endif
