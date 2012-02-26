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
 - Figure out why determineQuadrant isn't working...
 - Update moving objects
 - Move GameEngine stuff into here/integrate with GameEngine (Done?)
 - Prob lots more...
 */

const float VEC_OFFSET = 499.0f;//1000.0f;
const int QUADS_TO_CHECK = 4;

WorldGrid::WorldGrid(WorldData& world, Modules* modules)
   : m_world(world)
{
   m_modules = modules;
   
   m_currentQuadrant = 0;
   m_shouldUpdate = true;
   m_player = NULL;
   
   makeGrid();
}

WorldGrid::~WorldGrid()
{
   for (std::vector<Quadrant*>::iterator i = m_quadrants.begin(); i != m_quadrants.end(); i++) 
   {
      Quadrant* currQuad = *i;
      delete currQuad;
   }
}

void WorldGrid::setPlayer(Player *player)
{
   m_player = player;
   
   for (std::vector<Quadrant*>::iterator i = m_quadrants.begin(); i != m_quadrants.end(); i++) 
   {
      Quadrant* currQuad = *i;
      
      for (std::list<GameObject*>::iterator j = currQuad->m_gameObjects.begin(); j != currQuad->m_gameObjects.end(); j++) 
      {
         if (typeid(**j) == typeid(Turret)) 
         {
            Turret* enemy = ((Turret*)*j);
            
            enemy->setPlayer(m_player);
         }
      }
   }
}

void WorldGrid::placeInGrid(GameObject* gameObj, Object3d* obj3D)
{
   std::vector<Quadrant>::size_type ndx = determineQuadrant(gameObj->getPosition());
   
   m_quadrants.at(ndx)->m_gameObjects.push_back(gameObj);
   m_quadrants.at(ndx)->m_obj3Ds.push_back(obj3D);
}

void WorldGrid::placeInCurrQuadrant(GameObject* gameObj, Object3d* obj3D)
{
   m_quadrants.at(m_currentQuadrant)->m_gameObjects.push_back(gameObj);
   m_quadrants.at(m_currentQuadrant)->m_obj3Ds.push_back(obj3D);
}

void WorldGrid::tic(uint64_t dt)
{
   updateObjects();
   
   Camera& camera = ((Camera&)m_modules->gameEngine->getCamera());
   
   m_player->tic(dt, camera.getPosition(), camera.getUp(), camera.getForward());
   
   for (std::list<GameObject*>::iterator i = m_updateableObjs.begin(); i != m_updateableObjs.end(); i++) 
   {
      GameObject* currObj = *i;
      
      currObj->tic(dt);
   }
   
//   for (std::vector<Turret*>::iterator i = m_turrets.begin(); i != m_turrets.end(); i++) 
//   {
//      (*i)->tic(td);
//      
//      vec3 dirToPlayer = (*i)->getPosition() - m_player->getPosition();
//      
//      // Turret not currently firing, but I think it's because
//      //  the player starts too close to the turret
//      if ((*i)->isAlive() && dirToPlayer.Length() < 1000 && (*i)->shouldFire()) 
//      {
//         vec3 dirToPlayerNorm = dirToPlayer.Normalized();
//         
//         Bullet* bullet = 
//         new Bullet("models/cube.obj", "textures/test5.bmp", 
//                    m_modules, (*i)->getHeadPosition(), 
//                    -dirToPlayerNorm, 
//                    dirToPlayerNorm.Cross((*i)->getPosition()), *(*i));
//         
//         m_modules->renderingEngine->addObject3d(bullet);
//         m_gameObjects.push_back(bullet);
//         m_objects.push_back(bullet);
//         m_bulletList.push_back(bullet);
//      }
//   }
//   
//   vec3 dirEnemyToPlayer = m_enemyShip->getPosition() - m_player->getPosition();
//   if (dirEnemyToPlayer.Length() < 400 && m_enemyShip->shouldFire()) 
//   {
//      vec3 dirToPlayerNorm = dirEnemyToPlayer.Normalized();
//      
//      Bullet* bullet = 
//      new Bullet("models/cube.obj", "textures/test5.bmp", 
//                 m_modules, m_enemyShip->getLeftCannonPos(), 
//                 m_enemyShip->getAimForward(), 
//                 dirToPlayerNorm.Cross(m_enemyShip->getLeftCannonPos()), 
//                 *m_enemyShip, Bullet::defaultTimeToLive, 0.2f);
//      
//      m_modules->renderingEngine->addObject3d(bullet);
//      m_gameObjects.push_back(bullet);
//      m_objects.push_back(bullet);
//      m_bulletList.push_back(bullet);
//      
//      bullet = 
//      new Bullet("models/cube.obj", "textures/test5.bmp", 
//                 m_modules, m_enemyShip->getRightCannonPos(), 
//                 m_enemyShip->getAimForward(), 
//                 dirToPlayerNorm.Cross(m_enemyShip->getRightCannonPos()), 
//                 *m_enemyShip, Bullet::defaultTimeToLive, 0.2f);
//      
//      m_modules->renderingEngine->addObject3d(bullet);
//      m_gameObjects.push_back(bullet);
//      m_objects.push_back(bullet);
//      m_bulletList.push_back(bullet);
//   }
//   
//   
//   //Use Iterators!
//   //for (int i = 0; i < m_bulletList.size(); i++) {
//   for(std::vector<Bullet *>::iterator bulletIterator = m_bulletList.begin();
//       bulletIterator != m_bulletList.end();
//       bulletIterator++){ 
//      (*bulletIterator)->tic(td);
//      //Cull the bullet!
//      if(!(*bulletIterator)->isAlive()){
//         //m_bulletList.erase(bulletIterator);
//      }
//      
//   }
//   
//   for (std::vector<Missile *>::iterator missileIterator = m_missileList.begin(); 
//        missileIterator != m_missileList.end(); 
//        missileIterator++) {
//      (*missileIterator)->tic(td);
//      //Cull the missile!
//   }
   
   std::vector<GameObject*>::size_type quadrant = determineQuadrant(m_player->getPosition());
   
   if (quadrant != m_currentQuadrant) 
   {
      m_currentQuadrant = quadrant;
      m_shouldUpdate = true;
   }
}

void WorldGrid::checkCollisions()
{
   updateObjects();
   
   for (std::list<GameObject*>::iterator i = m_updateableObjs.begin(); i != m_updateableObjs.end(); i++) 
   {
      GameObject* currObj = *i;
      
      for (std::list<GameObject*>::iterator j = i; j != m_updateableObjs.end(); j++) 
      {
         // HACK to get iterators to work
         if (j != i) 
         {
            GameObject* toCheck = *j;
            
            // Check and do collisions
            if (currObj->isAlive() && toCheck->isAlive()
                && currObj->collidesWith(*toCheck)) 
            {
               currObj->doCollision(*toCheck);
            }         
         }
      }
   }
}

const Quadrant& WorldGrid::getCurrentQuadrant()
{
   return *m_quadrants.at(m_currentQuadrant);
}

std::list<IObject3d*> WorldGrid::getDrawableObjects()
{
   updateObjects();
   
   return m_drawableObjs;
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
   for (std::vector<PathPointData>::iterator i = m_world.path.begin(); i < m_world.path.end(); i++) 
   {
      PathPointData currPathPoint = *i;
      
      PathPointData nextPathPoint = i + 1 >= m_world.path.end() ? 
                                    *(m_world.path.begin()) : *(i + 1);
      
      Quadrant* quad = new Quadrant(currPathPoint, nextPathPoint);
      
      // Determine planes for later determining where enemies go
      vec3 avgUp = ((currPathPoint.up + nextPathPoint.up) / 2).Normalized();
      vec3 avgForward = ((currPathPoint.fwd + nextPathPoint.fwd) / 2).Normalized();
      vec3 sideVec = avgUp.Cross(avgForward).Normalized();
      
      vec3 leftPt  = quad->m_startPt.loc + (-sideVec * VEC_OFFSET);
      vec3 rightPt = quad->m_startPt.loc + (sideVec * VEC_OFFSET);
      vec3 downPt  = quad->m_startPt.loc + (-avgUp * VEC_OFFSET);
      vec3 upPt    = quad->m_startPt.loc + (avgUp * VEC_OFFSET);
      vec3 nearPt  = quad->m_startPt.loc;
      vec3 farPt   = quad->m_endPt.loc;
      
//      std::cout << "L: ";
//      leftPt.print();
//      std::cout << "R: "; 
//      rightPt.print();
//      std::cout << "D: ";
//      downPt.print();
//      std::cout << "U: ";
//      upPt.print();
//      std::cout << "N: ";
//      nearPt.print();
//      std::cout << "F: ";
//      farPt.print();
//      std::cout << "\n\n";
      
      lftPlane  = Plane::MakePlane(-sideVec, leftPt);
      rtPlane   = Plane::MakePlane(sideVec, rightPt);
      downPlane = Plane::MakePlane(-avgUp, downPt);
      upPlane   = Plane::MakePlane(avgUp, upPt);
      nearPlane = Plane::MakePlane(-avgForward, nearPt);
      farPlane  = Plane::MakePlane(avgForward, farPt);
      
      quad->m_bounds = new Cube(lftPlane, rtPlane, upPlane, downPlane, nearPlane, farPlane);
      
      // For all the units in the quadrant
      for (std::vector<UnitData>::iterator j = (*i).units.begin(); j != (*i).units.end(); j++) 
      {
         UnitData currUnit = *j;
         
         Vector3<float> position = currUnit.loc;
         Vector3<float> forward = currUnit.fwd;
         Vector3<float> up = currUnit.up;
         
         switch (currUnit.type) 
         {
            case UNIT_TURRET:
               Turret* newTurret = new Turret(m_player, 
                                              "models/turrethead.obj", "textures/test3.bmp", 
                                              "models/turretmiddle.obj", "textures/test3.bmp", 
                                              "models/turretbase.obj", "textures/test3.bmp", m_modules);
               
               newTurret->setPosition(position);
               newTurret->setForward(forward);
               newTurret->setUp(up);
               
               quad->m_gameObjects.push_back(newTurret);
               quad->m_obj3Ds.push_back(newTurret);
               break;
         }
      }
      
      // Scene Objects
      for (std::vector<PropData>::iterator j = (*i).props.begin(); j != (*i).props.end(); j++) 
      {
         PropData currProp = *j;
         
         vec3 position = currProp.loc;
         vec3 forward = currProp.fwd;
         vec3 up = currProp.up;
         
         SceneObject* sceneObj = new SceneObject("models/" + currProp.name, "textures/test3.bmp", position, forward, up, m_modules);
         
         quad->m_obj3Ds.push_back(sceneObj);
      }
      
      // Actually add the quadrant
      m_quadrants.push_back(quad);
   }
}

void WorldGrid::updateObjects()
{
   // If the information isn't 'cached'
   if (m_shouldUpdate) 
   {
      m_shouldUpdate = false;
      
      m_updateableObjs.clear();
      
      // Starting at the current quadrant, add all game objects in that and 
      // the next few quadrants
      for (std::vector<Quadrant*>::size_type i = m_currentQuadrant; 
           i < m_currentQuadrant + QUADS_TO_CHECK && i < m_quadrants.size(); i++) 
      {
         Quadrant* currQuad = m_quadrants.at(i);
         
         for (std::list<IObject3d*>::iterator j = currQuad->m_obj3Ds.begin(); j != currQuad->m_obj3Ds.end(); j++) 
         {
            //m_modules->renderingEngine->removeObject3d(*j);
         }
      }
      
      m_drawableObjs.clear();
      
      // Starting at the current quadrant, add all game objects in that and 
      // the next few quadrants
      for (std::vector<Quadrant*>::size_type i = m_currentQuadrant; 
           i < m_currentQuadrant + QUADS_TO_CHECK && i < m_quadrants.size(); i++) 
      {
         Quadrant* currQuad = m_quadrants.at(i);
         
         for (std::list<IObject3d*>::iterator j = currQuad->m_obj3Ds.begin(); j != currQuad->m_obj3Ds.end(); j++) 
         {
            m_modules->renderingEngine->addObject3d(*j);
         }
         
         m_updateableObjs.merge(m_quadrants.at(i)->m_gameObjects);
         m_drawableObjs.merge(m_quadrants.at(i)->m_obj3Ds);
      }
   }
}

std::vector<Quadrant>::size_type WorldGrid::determineQuadrant(const Vector3<float> pos)
{
   // Checking for NaNs
   //assert(pos.x == pos.x && pos.y == pos.y && pos.z == pos.z);
   if (!(pos.x == pos.x && pos.y == pos.y && pos.z == pos.z)) 
   {
      std::cerr << "NaN" << "\n";
   }
   
   std::vector<Quadrant>::size_type i;
   for (i = 0; i < m_quadrants.size(); i++) 
   {
      if (m_quadrants.at(i)->m_bounds->checkPoint(pos)) 
      {
         break;
      }
   }
   
   // If this triggers, we didn't find any quadrant that can house the object
   if(i == m_quadrants.size())
   {
      i = m_currentQuadrant;
   }
   
   return i;
}

#ifdef GAME_DEBUG
int WorldGrid::placeInGridDEBUG(GameObject* gameObj, Object3d* obj3D)
{
   std::vector<Quadrant>::size_type ndx = determineQuadrant(gameObj->getPosition());
   
   m_quadrants.at(ndx)->m_gameObjects.push_back(gameObj);
   m_quadrants.at(ndx)->m_obj3Ds.push_back(obj3D);
   
   return ndx;
}

int WorldGrid::placeInCurrQuadrantDEBUG(GameObject* gameObj, Object3d* obj3D)
{
   m_quadrants.at(m_currentQuadrant)->m_gameObjects.push_back(gameObj);
   m_quadrants.at(m_currentQuadrant)->m_obj3Ds.push_back(obj3D);
   
   return m_currentQuadrant;
}
#endif
