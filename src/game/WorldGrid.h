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
#include "Path.h"
#include "SceneObject.h"
#include "Turret.h"
#include "Player.h"
#include "Reticle.h"
#include "EnemyShip.h"
#include "EnemyGunship.h"

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
   WorldGrid(Path& path, WorldData& world, Modules* modules, Player* player,
	     vector<Bullet*>* bulletList);
   
   /**
    * Frees the grid in memory.
    */
   ~WorldGrid();
   
   /**
    * Set the player pointer in WorldGrid
    */
   void setPlayer(Player* player);
   
   Quadrant getCurrentQuadrant();
   
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
    * Returns the list of Object3Ds that will be needed
    */
   std::list<IObject3d*> getDrawableObjects();
   
#ifndef GAME_DEBUG
private:
#endif
   WorldData& m_world;
   Path& m_path;
   Player* m_player;
   Modules* m_modules;
   bool m_shouldUpdate;
   vector<Bullet*>* m_bulletList;
   std::vector<Quadrant*>::size_type m_currentQuadrant;
   
   /**
    * Runs through the inital WorldData and creates all the Quadrants with and objects
    */
   void makeGrid();
};

#endif
