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

WorldGrid::WorldGrid(Path& path, WorldData& world, Modules* modules, Player* player)
   : m_path(path), m_world(world), m_modules(modules), m_player(player)
{
   m_currentQuadrant = 0;
   m_shouldUpdate = true;
   
   makeGrid();
}

WorldGrid::~WorldGrid()
{
   
}

Quadrant WorldGrid::getCurrentQuadrant()
{
   return m_path.getCurrentQuadrant();
}

void WorldGrid::tic(uint64_t dt, Path* path)
{
   Quadrant quad = m_path.getCurrentQuadrant();
   
   for (std::list<GameObject*>::iterator i = quad.m_gameObjects.begin(); i != quad.m_gameObjects.end(); i++) 
   {
      GameObject* currObj = *i;
      
      currObj->tic(dt);
      
//      vec3 dirToPlayer = (*i)->getPosition() - m_player->getPosition();
//      
//      if (typeid(**i) == typeid(Turret)) 
//      {
//         Turret* turret = ((Turret*)*i);
//         
//         // Turret not currently firing, but I think it's because
//         // the player starts too close to the turret
//         if (turret->isAlive() && dirToPlayer.Length() < 1500 && turret->shouldFire())
//         {
//            vec3 dirToPlayerNorm = dirToPlayer.Normalized();
//            
//            Bullet* bullet = 
//            new Bullet("models/lance.obj", "textures/test5.bmp", 
//                       m_modules, turret->getHeadPosition(), 
//                       -dirToPlayerNorm, 
//                       dirToPlayerNorm.Cross(turret->getPosition()), *(turret));
//            
//            m_path.addToQuadrants(bullet->getPosition(), bullet, bullet);
//         }
//      }
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
}

void WorldGrid::checkCollisions()
{
   Quadrant quad = m_path.getCurrentQuadrant();
   
   for (std::list<GameObject*>::iterator i = quad.m_gameObjects.begin(); i != quad.m_gameObjects.end(); i++) 
   {
      GameObject* currObj = *i;
      
      for (std::list<GameObject*>::iterator j = i; j != quad.m_gameObjects.end(); j++) 
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

std::list<IObject3d*> WorldGrid::getDrawableObjects()
{
   return m_path.getCurrentQuadrant().m_obj3Ds;
}

void WorldGrid::makeGrid()
{
   // Connect the points into quadrants
   for (std::vector<PathPointData>::iterator i = m_world.path.begin(); i < m_world.path.end(); i++) 
   {
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
               Turret* newTurret = new Turret(m_player, m_modules);
               
               newTurret->setPosition(position);
               newTurret->setForward(forward);
               newTurret->setUp(up);
               
               m_path.addToQuadrants(position, newTurret, newTurret);
               m_modules->renderingEngine->addObject3d(newTurret);
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
         
         SceneObject* sceneObj = new SceneObject("models/" + currProp.name + ".obj", "textures/" + currProp.name + ".bmp", position, forward, up, m_modules);
         
         m_path.addToQuadrants(position, NULL, sceneObj);
         m_modules->renderingEngine->addObject3d(sceneObj);
      }
   }
}