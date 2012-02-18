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
#include "Cube.h"
#include "Turret.h"
#include "Player.h"
#include "EnemyShip.h"
#include "EnemyGunship.h"

/**
 *
 */
struct Quadrant
{
   Quadrant(vec3 startPt, vec3 endPt)
   {
      m_startPt = startPt;
      m_endPt = endPt;
      
      
   }
   
   ~Quadrant()
   {
      delete m_bounds;
   }
   
   std::list<GameObject*> m_gameObjects;
   vec3 m_startPt;
   vec3 m_endPt;
   
   Cube* m_bounds;
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
   WorldGrid(WorldData& world, Modules* modules);
   
   /**
    * Frees the grid in memory.
    */
   ~WorldGrid();
   
   /**
    * Set the player pointer in WorldGrid
    */
   void setPlayer(Player* player);
   
   /**
    * Place a game object in the WorldGrid.
    * This should only be needed for Enemy Ships, since they aren't 
    * currently exported.
    * @param gameObj the object to add to the world
    * @param objPosition the object's position in the world
    */
   void placeInGrid(GameObject* gameObj, vec3 objPosition);
   
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
   Player* m_player;
   Modules* m_modules;
   bool m_shouldUpdate;
   
   std::vector<Quadrant>::size_type m_currentQuadrant;
   std::list<GameObject*> m_updateableObjs;
   
   /**
    *
    */
   void makeGrid();
   
   /**
    *
    */
   std::vector<Quadrant>::size_type determineQuadrant(const Vector3<float> pos);
   
   /**
    *
    */
   void placeTurrets();
   
};

#endif
