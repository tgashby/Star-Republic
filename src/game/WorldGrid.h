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
#include "SceneObject.h"
#include "Turret.h"
#include "Player.h"
#include "Reticle.h"
#include "EnemyShip.h"
#include "EnemyGunship.h"

/**
 *
 */
struct Quadrant
{
   Quadrant(PathPointData startPt, PathPointData endPt)
   : m_startPt(startPt), m_endPt(endPt)
   {
      
   }
   
   ~Quadrant()
   {
      delete m_bounds;
   }
   
   std::list<GameObject*> m_gameObjects;
   std::list<IObject3d*> m_obj3Ds;
   PathPointData m_startPt;
   PathPointData m_endPt;
   
   Cube* m_bounds;
};

/**
 * WorldGrid represents the world as a uniform spatial subdivision.
 * It is a list of cubes encompassing the path and all its objects.
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
    *
    */
   void makeGrid();
   
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
   void placeInGrid(GameObject* gameObj, Object3d* obj3D);
   
   /**
    *
    */
   void placeInCurrQuadrant(GameObject* gameObj, Object3d* obj3D);
   
   /**
    *
    */
   void tic(uint64_t dt);
   
   /**
    * 
    */
   void checkCollisions();
   
   /**
    *
    */
   const Quadrant& getCurrentQuadrant();
   
   /**
    *
    */
   std::list<IObject3d*> getDrawableObjects();
   
private:
   WorldData& m_world;
   std::vector<Quadrant*> m_quadrants;
   Player* m_player;
   Modules* m_modules;
   bool m_shouldUpdate;
   
   
   /**
    * 
    */
   void updateObjects();
   
   std::vector<Quadrant*>::size_type m_currentQuadrant;
   std::list<GameObject*> m_updateableObjs;
   std::list<IObject3d*> m_drawableObjs;
   
   /**
    *
    */
   std::vector<Quadrant*>::size_type determineQuadrant(const Vector3<float> pos);
   
};

#endif
