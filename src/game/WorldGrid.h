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
 * Quadrant structure, representing the area between two points
 */
struct Quadrant
{
   /**
    * Quadrant constructor
    * @param startPt the first point
    * @param endPt the second point
    */
   Quadrant(PathPointData startPt, PathPointData endPt)
   : m_startPt(startPt), m_endPt(endPt)
   {
   }
   
   /**
    * Cleans up the Cube*
    */
   ~Quadrant()
   {
      delete m_bounds;
   }
   
   std::list<GameObject*> m_gameObjects;
   std::list<IObject3d*> m_obj3Ds;
   PathPointData m_startPt;
   PathPointData m_endPt;
   
   /**
    * Cube representing the space between the two points.
    * Used in calculating whether an object is in the quadrant or not.
    */
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
    * Set the player pointer in WorldGrid
    */
   void setPlayer(Player* player);
   
   /**
    * Place a game object in the WorldGrid.
    * This should only be needed for Enemy Ships, since they aren't 
    * currently exported.
    * @param gameObj the object to add to the world
    * @param obj3D the same object as the first, but needed to add to
    * drawable objects    
    */
   void placeInGrid(GameObject* gameObj, Object3d* obj3D);
   
   /**
    * Place a game object in the current quadrant.
    * This will be used for bullets, missiles, etc.
    * @param gameObj the game object to place
    * @param obj3D the same object as the first, but needed to add to
    * drawable objects
    */
   void placeInCurrQuadrant(GameObject* gameObj, Object3d* obj3D);
   
#ifdef GAME_DEBUG
   int placeInGridDEBUG(GameObject* gameObj, Object3d* obj3D);

   int placeInCurrQuadrantDEBUG(GameObject* gameObj, Object3d* obj3D);
#endif
   
   /**
    * Updates all objects that are in the current quadrants.
    * @param dt the time passed since the last update.
    */
   void tic(uint64_t dt);
   
   /**
    * Checks all objects in the current quadrants for collisions.
    */
   void checkCollisions();
   
   /**
    * Returns the current quadrant, used to get at starting and ending points
    */
   const Quadrant& getCurrentQuadrant();
   
   /**
    * Returns the list of Object3Ds that will be needed
    */
   std::list<IObject3d*> getDrawableObjects();
   
#ifndef GAME_DEBUG
private:
#endif
   WorldData& m_world;
   std::vector<Quadrant*> m_quadrants;
   Player* m_player;
   Modules* m_modules;
   bool m_shouldUpdate;   
   std::vector<Quadrant*>::size_type m_currentQuadrant;
   std::list<GameObject*> m_updateableObjs;
   std::list<IObject3d*> m_drawableObjs;
   
   /**
    * Runs through the inital WorldData and creates all the Quadrants with and objects
    */
   void makeGrid();
   
   /**
    * Updates the visible/updateable game objects if needed
    */
   void updateObjects();

   /**
    * Determines the quadrant number in the vector that the point is located in.
    */
   std::vector<Quadrant*>::size_type determineQuadrant(const Vector3<float> pos);
   
};

#endif
