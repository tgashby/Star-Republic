//
//  WorldGrid.cpp
//  StarRepublic
//
//  Created by Taggart Ashby on 2/14/12.
//  Copyright 2012 476 Proj. All rights reserved.
//

#include <iostream>

#include "WorldGrid.h"

/*
 TODO:
 - Update makeGrid and placeTurrets to use new style
 - Update player
 - Position player
 - Determine when to update (m_shouldUpdate)
 - Update moving objects
 - Keep track of which quadrant we're in
 - Make a Cube for each quadrant when it's made
 - Implement determineQuadrant, using Cube.checkPoint(objPos) against
   each quadrant's Cube
 - Move GameEngine stuff into here/integrate with GameEngine
 - Prob lots more...
 */

WorldGrid::WorldGrid(WorldData& world, Modules* modules)
   : m_world(world)
{
   m_modules = modules;
   
   m_currentQuadrant = 0;
   
   makeGrid();
   placeTurrets();
}

WorldGrid::~WorldGrid()
{
   
}

void WorldGrid::setPlayer(Player *player)
{
   m_player = player;
}


void WorldGrid::placeInGrid(GameObject* gameObj, vec3 objPosition)
{
   std::vector<Quadrant>::size_type ndx = determineQuadrant(objPosition);
   
   m_quadrants.at(ndx).m_gameObjects.push_back(gameObj);
}

std::list<GameObject*> WorldGrid::getUpdateableObjects()
{
   if (m_shouldUpdate) 
   {
      m_updateableObjs.clear();
      
      // Starting at the current quadrant, add all game objects in that and 
      // the next 4 quadrants
      for (std::vector<Quadrant>::size_type i = m_currentQuadrant; 
           i < m_currentQuadrant + 4 && i < m_quadrants.size(); i++) 
      {
         m_updateableObjs.merge(m_quadrants.at(i).m_gameObjects);
      }
   }
   
   return m_updateableObjs;
}

void WorldGrid::makeGrid()
{
   Plane lftPlane;
   Plane rtPlane;
   Plane downPlane;
   Plane upPlane;
   Plane nearPlane;
   Plane farPlane;
   
   // Connect the points into quadrants
   for (std::vector<vec3>::iterator i = m_world.path.begin(); i != m_world.path.end(); i += 2) 
   {
      Quadrant quad(*i, *(i+1));
      
      
      
      quad.m_bounds = new Cube(lftPlane, rtPlane, upPlane, downPlane, nearPlane, farPlane);
      
      m_quadrants.push_back(quad);
   }
}

std::vector<Quadrant>::size_type WorldGrid::determineQuadrant(const Vector3<float> pos)
{
   
}

void WorldGrid::placeTurrets()
{
   for (std::vector< Vector3<float> >::iterator i = m_world.turrets.begin(); 
        i != m_world.turrets.end(); i += 3)
   {
      Vector3<float> position = *i;
      Vector3<float> forward = *(i + 1);
      Vector3<float> up = *(i + 2);
      
      std::vector<Quadrant>::size_type ndx = determineQuadrant(position);
      
      // Loc, forward, up
      Turret* newTurret = new Turret(*m_player, "models/turrethead.obj", 
                                     "textures/test3.bmp", "models/turretmiddle.obj", 
                                     "textures/test3.bmp", "models/turretbase.obj", 
                                     "textures/test3.bmp", m_modules);
      
      newTurret->setPosition(position);
      newTurret->setForward(forward);
      newTurret->setUp(up);
      
      m_quadrants.at(ndx).m_gameObjects.push_back(newTurret);
   }
}
